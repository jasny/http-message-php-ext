/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - Factory methods                         |
  +----------------------------------------------------------------------+
  | Copyright (c) 2019 Arnold Daniels                                    |
  +----------------------------------------------------------------------+
  | Permission is hereby granted, free of charge, to any person          |
  | obtaining a copy of this software and associated documentation files |
  | (the "Software"), to deal in the Software without restriction,       |
  | including without limitation the rights to use, copy, modify, merge, |
  | publish, distribute, sublicense, and/or sell copies of the Software, |
  | and to permit persons to whom the Software is furnished to do so,    |
  | subject to the following conditions:                                 |
  |                                                                      |
  | The above copyright notice and this permission notice shall be       |
  | included in all copies or substantial portions of the Software.      |
  |                                                                      |
  | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      |
  | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   |
  | MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                |
  | NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  |
  | BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   |
  | ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    |
  | CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     |
  | SOFTWARE.                                                            |
  +----------------------------------------------------------------------+
  | Author: Arnold Daniels <arnold@jasny.net>                            |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_http_message.h"
#include "macros.h"
#include "response.h"
#include "stream.h"
#include "uploaded_file.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_string.h"
#include "ext/psr/psr_http_factory.h"
#include "ext/spl/spl_exceptions.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Factory_ce = NULL;

int uri_param_as_object(zval *uri)
{
    zval uri_str;
    zend_class_entry *uri_interface = HTTP_MESSAGE_PSR_INTERFACE("uri");

    if (uri_interface == NULL) {
        zend_throw_error(NULL, "Psr\\Http\\Message\\UriInterface not found");
        return FAILURE;
    }

    if (Z_TYPE_P(uri) == IS_STRING) {
        ZVAL_COPY(&uri_str, uri);
        NEW_OBJECT_CONSTRUCT(uri, HttpMessage_Uri_ce, 1, &uri_str);
    } else if (UNEXPECTED(Z_TYPE_P(uri) != IS_OBJECT || !instanceof_function(Z_OBJCE_P(uri), uri_interface))) {
        custom_parameter_type_error(1, "a string or object that implements Psr\\Http\\Message\\UriInterface", uri);
        return FAILURE;
    }

    return SUCCESS;
}

PHP_METHOD(Factory, createRequest)
{
    zend_string *method = NULL;
    zval *uri = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_STR(method)
        Z_PARAM_ZVAL(uri)
    ZEND_PARSE_PARAMETERS_END();

    if (uri_param_as_object(uri) == FAILURE) return;

    NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Request_ce, 0);
    zend_update_property_str(HttpMessage_Request_ce, return_value, ZEND_STRL("method"), method);
    zend_update_property(HttpMessage_Request_ce, return_value, ZEND_STRL("uri"), uri);
}

PHP_METHOD(Factory, createResponse)
{
    zend_long code = 200;
    zend_bool code_is_null = 0;
    zend_string *phrase = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG_EX(code, code_is_null, 0, 0)
        Z_PARAM_STR(phrase)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Response_ce, 0);
    response_set_status(return_value, !code_is_null ? code : 200, phrase);
}

PHP_METHOD(Factory, createServerRequest)
{
    zend_string *method = NULL;
    zval *uri = NULL, *serverParams = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 3)
        Z_PARAM_STR(method)
        Z_PARAM_ZVAL(uri)
        Z_PARAM_OPTIONAL
        Z_PARAM_ARRAY_EX(serverParams, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    if (uri_param_as_object(uri) == FAILURE) return;

    if (serverParams == NULL) {
        NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_ServerRequest_ce, 0);
    } else {
        NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_ServerRequest_ce, 1, serverParams);
    }

    zend_update_property_str(HttpMessage_ServerRequest_ce, return_value, ZEND_STRL("method"), method);
    zend_update_property(HttpMessage_ServerRequest_ce, return_value, ZEND_STRL("uri"), uri);
}

PHP_METHOD(Factory, createStream)
{
    php_stream *stream;
    zval resource;
    zend_string *content = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR(content)
    ZEND_PARSE_PARAMETERS_END();

    if (open_temp_stream(&resource) == FAILURE) return;

    if (content != NULL && ZSTR_LEN(content) > 0) {
        php_stream_from_zval(stream, &resource);
        php_stream_write(stream, ZSTR_VAL(content), ZSTR_LEN(content));
    }

    NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Stream_ce, 1, &resource);
}

PHP_METHOD(Factory, createStreamFromFile)
{
    char *file = NULL, *mode = NULL;
    size_t file_len = 0, mode_len = 0;
    php_stream *stream;
    zval resource;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
        Z_PARAM_STRING(file, file_len)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(mode, mode_len)
    ZEND_PARSE_PARAMETERS_END();

    file[file_len] = '\0';
    stream = php_stream_open_wrapper(file, mode != NULL ? mode : "r", 0, NULL);

    if (stream == NULL) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to open '%s' stream", file);
        return;
    }

    php_stream_to_zval(stream, &resource);
    NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Stream_ce, 1, &resource);
}

PHP_METHOD(Factory, createStreamFromResource)
{
    zval *resource = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ZVAL(resource)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Stream_ce, 1, resource);
}

PHP_METHOD(Factory, createUploadedFile)
{
    zval *stream = NULL;
    zend_long size = -1, error = 0;
    zend_string *clientFilename = NULL, *clientMediaType = NULL;
    zend_bool size_is_null = 1;
    zend_class_entry *stream_interface = HTTP_MESSAGE_PSR_INTERFACE("stream");

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 5)
        Z_PARAM_OBJECT_OF_CLASS(stream, stream_interface)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG_EX(size, size_is_null, 1, 0)
        Z_PARAM_LONG(error)
        Z_PARAM_STR_EX(clientFilename, 1, 0)
        Z_PARAM_STR_EX(clientMediaType, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT(return_value, HttpMessage_UploadedFile_ce);
    construct_uploaded_file(return_value, stream, NULL, size_is_null ? -1 : size, error, clientFilename,
            clientMediaType, -1);
}

PHP_METHOD(Factory, createUri)
{
    zend_string *uri = NULL;
    zval zuri;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 0)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR_EX(uri, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    if (uri == NULL) {
        NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Request_ce, 0);
    } else {
        ZVAL_STR(&zuri, uri);
        NEW_OBJECT_CONSTRUCT(return_value, HttpMessage_Request_ce, 1, &zuri);
    }
}

/* Define HttpMessage\Factory class */

static const zend_function_entry request_functions[] = {
    HTTP_MESSAGE_ME_EX(Factory, RequestFactory, createRequest)
    HTTP_MESSAGE_ME_EX(Factory, ResponseFactory, createResponse)
    HTTP_MESSAGE_ME_EX(Factory, ServerRequestFactory, createServerRequest)
    HTTP_MESSAGE_ME_EX(Factory, StreamFactory, createStream)
    HTTP_MESSAGE_ME_EX(Factory, StreamFactory, createStreamFromFile)
    HTTP_MESSAGE_ME_EX(Factory, StreamFactory, createStreamFromResource)
    HTTP_MESSAGE_ME_EX(Factory, UploadedFileFactory, createUploadedFile)
    HTTP_MESSAGE_ME_EX(Factory, UriFactory, createUri)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_factory)
{
    zend_class_entry ce;
    zend_class_entry *request_factory = HTTP_MESSAGE_PSR_INTERFACE("requestfactory");
    zend_class_entry *response_factory = HTTP_MESSAGE_PSR_INTERFACE("responsefactory");
    zend_class_entry *serverrequest_factory = HTTP_MESSAGE_PSR_INTERFACE("serverrequestfactory");
    zend_class_entry *stream_factory = HTTP_MESSAGE_PSR_INTERFACE("streamfactory");
    zend_class_entry *uploadedfile_factory = HTTP_MESSAGE_PSR_INTERFACE("uploadedfilefactory");
    zend_class_entry *uri_factory = HTTP_MESSAGE_PSR_INTERFACE("urifactory");

    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(request_factory, "Factory", "RequestFactory");
    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(response_factory, "Factory", "ResponseFactory");
    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(serverrequest_factory, "Factory", "ServerRequestFactory");
    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(stream_factory, "Factory", "StreamFactory");
    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(uploadedfile_factory, "Factory", "UploadedFileFactory");
    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(uri_factory, "Factory", "UriFactory");

    if (UNEXPECTED(
            HttpMessage_Request_ce == NULL || HttpMessage_Response_ce == NULL || HttpMessage_ServerRequest_ce == NULL ||
            HttpMessage_Stream_ce == NULL || HttpMessage_UploadedFile_ce == NULL || HttpMessage_Uri_ce == NULL
    )) {
        return FAILURE;
    }

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Factory", request_functions);

    HttpMessage_Factory_ce = zend_register_internal_class(&ce);
    zend_class_implements(HttpMessage_Factory_ce, 6, request_factory, response_factory, serverrequest_factory,
            stream_factory, uploadedfile_factory, uri_factory);

    return SUCCESS;
}

#endif
