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
#include "uploaded_file.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_string.h"
#include "ext/standard/php_string.h"
#include "ext/psr/psr_http_message.h"
#include "ext/psr/psr_http_factory.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Factory_ce = NULL;

PHP_METHOD(Factory, createRequest)
{
    zval *method, *uri;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_ZVAL_STRING(method)
        Z_PARAM_ZVAL(uri)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT_0(return_value, HttpMessage_Request_ce);
    zend_call_method_with_1_params(return_value, HttpMessage_Request_ce, NULL, "withMethod", return_value, method);
    zend_call_method_with_1_params(return_value, HttpMessage_Request_ce, NULL, "withUri", return_value, uri);
}

PHP_METHOD(Factory, createResponse)
{
    zval *code = NULL, *reasonPhrase = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL_LONG(code)
        Z_PARAM_ZVAL_STRING(reasonPhrase)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT_0(return_value, HttpMessage_Response_ce);

    if (code != NULL) {
        zend_call_method_with_2_params(return_value, HttpMessage_Response_ce, NULL, "withStatus", return_value, code,
               reasonPhrase);
    }
}

PHP_METHOD(Factory, createServerRequest)
{
    zval *method, *uri, *serverParams = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 3)
        Z_PARAM_ZVAL_STRING(method)
        Z_PARAM_ZVAL(uri)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL_ARRAY_EX(serverParams, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT_1(return_value, HttpMessage_ServerRequest_ce, serverParams);
    zend_call_method_with_1_params(return_value, HttpMessage_ServerRequest_ce, NULL, "withMethod", return_value,
            method);
    zend_call_method_with_1_params(return_value, HttpMessage_ServerRequest_ce, NULL, "withUri", return_value, uri);
}

PHP_METHOD(Factory, createStream)
{
    zval *content;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ZVAL_STRING(content)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT_0(return_value, HttpMessage_Stream_ce);
    zend_call_method_with_1_params(return_value, HttpMessage_Stream_ce, NULL, "write", NULL, content);
}

PHP_METHOD(Factory, createStreamFromFile)
{
    char *file;
    size_t file_len;
    php_stream *stream;
    zval resource;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(file, file_len)
    ZEND_PARSE_PARAMETERS_END();

    file[file_len] = '\0';
    stream = php_stream_open_wrapper(file, "r+", 0, NULL);

    if (stream == NULL) {
        zend_throw_error(NULL, "Failed to open 'php://temp' stream");
        return;
    }

    php_stream_to_zval(stream, &resource);
    NEW_OBJECT_CONSTRUCT_1(return_value, HttpMessage_Stream_ce, &resource);
}

PHP_METHOD(Factory, createStreamFromResource)
{
    zval *resource;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ZVAL(resource)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT_1(return_value, HttpMessage_Stream_ce, resource);
}

PHP_METHOD(Factory, createUploadedFile)
{
    zval *stream = NULL;
    zend_long size = -1, error = 0;
    zend_string *clientFilename = NULL, *clientMediaType = NULL;
    zend_bool size_is_null = 1;
    zend_class_entry *stream_interface = get_internal_ce(ZEND_STRL("psr\\http\\message\\streaminterface"));

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
    zval *uri = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 0)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL_STRING_EX(uri, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    NEW_OBJECT_CONSTRUCT_1(return_value, HttpMessage_Request_ce, uri);
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
    zend_class_entry *request_factory = get_internal_ce(ZEND_STRL("psr\\http\\message\\requestfactoryinterface"));
    zend_class_entry *response_factory = get_internal_ce(ZEND_STRL("psr\\http\\message\\responsefactoryinterface"));
    zend_class_entry *serverrequest_factory
        = get_internal_ce(ZEND_STRL("psr\\http\\message\\serverrequestfactoryinterface"));
    zend_class_entry *stream_factory = get_internal_ce(ZEND_STRL("psr\\http\\message\\streamfactoryinterface"));
    zend_class_entry *uploadedfile_factory
        = get_internal_ce(ZEND_STRL("psr\\http\\message\\uploadedfilefactoryinterface"));
    zend_class_entry *uri_factory = get_internal_ce(ZEND_STRL("psr\\http\\message\\urifactoryinterface"));

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
