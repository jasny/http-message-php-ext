/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - ServerRequest class                     |
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

#include <php.h>
#include "ext/spl/spl_exceptions.h"
#include "ext/psr/psr_http_message.h"
#include "php_http_message.h"
#include "macros.h"
#include "uploaded_file.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_ServerRequest_ce = NULL;

int assert_uploaded_files(HashTable *array)
{
    zval *entry;
    zend_class_entry *interface = HTTP_MESSAGE_PSR_INTERFACE("uploadedfile");

    if (interface == NULL) {
        zend_throw_error(NULL, "Psr\\Http\\Message\\UploadedFileInterface not found");
        return FAILURE;
    }

    ZEND_HASH_FOREACH_VAL(array, entry) {
        if (Z_TYPE_P(entry) == IS_OBJECT && EXPECTED(instanceof_function(Z_OBJCE_P(entry), interface))) {
            continue;
        }

        if (UNEXPECTED(Z_TYPE_P(entry) != IS_ARRAY || assert_uploaded_files(Z_ARR_P(entry)) == FAILURE)) { // recursion
            zend_throw_exception(spl_ce_InvalidArgumentException,
                     "Expected all elements to implement Psr\\Http\\Message\\UploadedFileInterface", 0);
            return FAILURE;
        }
    } ZEND_HASH_FOREACH_END();

    return SUCCESS;
}

void add_header_from_param(HashTable *headers, char *key, size_t keylen, zval *val)
{
    zval *new_header, valcpy;
    char header[256];
    size_t i;
    zend_bool lower; // Turn next char to lower case

    if (UNEXPECTED(keylen > 255)) {
        php_error_docref(NULL, E_WARNING, "Ignoring header '%s'; field to long", key);
        return;
    }

    strncpy(header, key, keylen);
    header[keylen] = '\0';

    for (i = 0, lower = 0; i < keylen; i++) {
        if (lower && header[i] >= 65 && header[i] <= 90) { // is ASCII A-Z
            header[i] += 32; // to lower case
        }

        if (header[i] == '_') {
            header[i] = '-';
            lower = 0;
        } else {
            lower = 1;
        }
    }

    new_header = zend_hash_str_add_empty_element(headers, header, keylen);

    array_init(new_header);
    ZVAL_COPY(&valcpy, val);
    add_next_index_zval(new_header, &valcpy);
}

void init_headers_from_params(zval *object, HashTable *serverParams)
{
    zval rv, *val;
    HashTable *headers;
    zend_long index;
    zend_string *key;

    headers = Z_ARR_P(zend_read_property(HttpMessage_Message_ce, PROPERTY_ARG(object), ZEND_STRL("headers"), 0, &rv));

    ZEND_HASH_FOREACH_KEY_VAL(serverParams, index, key, val) {
        (void)index; /* NOOP, to avoid unused warning */
        if (UNEXPECTED(key == NULL)) continue;

        if (ZSTR_LEN(key) > 5 && strncmp("HTTP_", ZSTR_VAL(key), 5) == 0 && EXPECTED(Z_TYPE_P(val) == IS_STRING)) {
            add_header_from_param(headers, ZSTR_VAL(key) + 5, ZSTR_LEN(key) - 5, val);
        }
    } ZEND_HASH_FOREACH_END();

    val = zend_hash_str_find(serverParams, ZEND_STRL("CONTENT_TYPE"));
    if (val != NULL && EXPECTED(Z_TYPE_P(val) == IS_STRING)) {
        add_header_from_param(headers, ZEND_STRL("CONTENT_TYPE"), val);
    }

    val = zend_hash_str_find(serverParams, ZEND_STRL("CONTENT_LENGTH"));
    if (val != NULL && EXPECTED(Z_TYPE_P(val) == IS_LONG)) {
        add_header_from_param(headers, ZEND_STRL("CONTENT_LENGTH"), val);
    }
}

void init_uri_from_params(zval *object, HashTable *serverParams)
{
    zval rv, *uri, *tmp, *request_target, *protocol, *https, *user, *pass;
    zend_long port = -1, default_port = -1;
    zend_bool is_http;

    uri = zend_read_property(HttpMessage_Request_ce, PROPERTY_ARG(object), ZEND_STRL("uri"), 0, &rv);

    request_target = zend_hash_str_find(serverParams, ZEND_STRL("REQUEST_URI"));
    zend_call_method(PROPERTY_ARG(uri), HttpMessage_Uri_ce, &HttpMessage_Uri_ce->constructor, ZEND_STRL("__construct"), NULL,
             request_target == NULL ? 0 : 1, request_target, NULL);

    COPY_PROPERTY_FROM_ARRAY(serverParams, "HTTP_HOST", uri, HttpMessage_Uri_ce, "host", IS_STRING, tmp);
    COPY_PROPERTY_FROM_ARRAY(serverParams, "QUERY_STRING", uri, HttpMessage_Uri_ce, "query", IS_STRING, tmp);

    tmp = zend_hash_str_find(serverParams, ZEND_STRL("SERVER_PORT"));
    if (tmp != NULL && EXPECTED(Z_TYPE_P(tmp) == IS_LONG && Z_LVAL_P(tmp) > 0)) {
        port = Z_LVAL_P(tmp);
    }

    protocol = zend_hash_str_find(serverParams, ZEND_STRL("SERVER_PROTOCOL"));
    https = zend_hash_str_find(serverParams, ZEND_STRL("HTTPS"));
    is_http = protocol != NULL && Z_TYPE_P(protocol) == IS_STRING
        ? (strncmp("HTTP/", Z_STRVAL_P(protocol), 5) == 0)
        : (port > 0 && port == (Z_STRCMP(https, "off", 0) == 0 ? 80 : 443));

    if (!is_http) {
        // do nothing
    } else if (Z_STRCMP(https, "off", 0) == 0) {
        default_port = 80;
        zend_update_property_stringl(HttpMessage_Uri_ce, PROPERTY_ARG(uri), ZEND_STRL("scheme"), ZEND_STRL("http"));
    } else {
        default_port = 443;
        zend_update_property_stringl(HttpMessage_Uri_ce, PROPERTY_ARG(uri), ZEND_STRL("scheme"), ZEND_STRL("https"));
    }

    if (port != default_port && port > 0) {
        zend_update_property_long(HttpMessage_Uri_ce, PROPERTY_ARG(uri), ZEND_STRL("port"), port);
    }

    user = zend_hash_str_find(serverParams, ZEND_STRL("PHP_AUTH_USER"));
    if (user != NULL) {
        pass = zend_hash_str_find(serverParams, ZEND_STRL("PHP_AUTH_PASS"));
        uri_set_userinfo(uri, Z_STRVAL_P(user), Z_STRLEN_P(user), Z_STRVAL_P_NULL(pass), Z_STRLEN_P_NULL(pass));
    }
}

/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageServerRequest_construct, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, serverParams, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, cookieParams, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, queryParams, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, postParams, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, files, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(ServerRequest, __construct)
{
    zval rv, *uploadedFiles, *val;
    zval *serverParams = NULL, *cookieParams = NULL, *queryParams = NULL, *post = NULL, *files = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 5)
        Z_PARAM_OPTIONAL
        Z_PARAM_ARRAY_EX(serverParams, 1, 0)
        Z_PARAM_ARRAY_EX(cookieParams, 1, 0)
        Z_PARAM_ARRAY_EX(queryParams, 1, 0)
        Z_PARAM_ARRAY_EX(post, 1, 0)
        Z_PARAM_ARRAY_EX(files, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    /* parent::__construct() */
    zend_call_method_with_0_params(
            PROPERTY_ARG(getThis()), HttpMessage_Request_ce, &HttpMessage_Request_ce->constructor, "__construct", NULL
    );

    SET_ARRAY_PROPERTY(HttpMessage_ServerRequest_ce, "serverParams", serverParams, rv);
    SET_ARRAY_PROPERTY(HttpMessage_ServerRequest_ce, "cookieParams", cookieParams, rv);
    SET_ARRAY_PROPERTY(HttpMessage_ServerRequest_ce, "queryParams", queryParams, rv);

    if (files != NULL) {
        uploadedFiles = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("uploadedFiles"), 0, &rv);
        create_uploaded_files(uploadedFiles, Z_ARR_P(files));
    } else {
        INIT_ARRAY_PROPERTY(HttpMessage_ServerRequest_ce, "uploadedFiles", rv);
    }

    if (post != NULL) {
        zend_update_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("parsedBody"), post);
    }

    if (serverParams != NULL) {
        COPY_PROPERTY_FROM_ARRAY(Z_ARR_P(serverParams), "REQUEST_METHOD", getThis(), HttpMessage_ServerRequest_ce,
                "method", IS_STRING, val);
        init_headers_from_params(getThis(), Z_ARR_P(serverParams));
        init_uri_from_params(getThis(), Z_ARR_P(serverParams));
    }

    INIT_ARRAY_PROPERTY(HttpMessage_ServerRequest_ce, "attributes", rv);
}


/* serverParams */

PHP_METHOD(ServerRequest, getServerParams)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("serverParams"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}


/* cookieParams */

PHP_METHOD(ServerRequest, getCookieParams)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("cookieParams"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withCookieParams)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY(value);
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(PROPERTY_ARG(getThis())));

    zend_update_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("cookieParams"), value);
}


/* queryParams */

PHP_METHOD(ServerRequest, getQueryParams)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("queryParams"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withQueryParams)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY(value);
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(PROPERTY_ARG(getThis())));

    zend_update_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("queryParams"), value);
}


/* uploadedFiles */

PHP_METHOD(ServerRequest, getUploadedFiles)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("uploadedFiles"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withUploadedFiles)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY(value);
    ZEND_PARSE_PARAMETERS_END();

    if (UNEXPECTED(assert_uploaded_files(Z_ARR_P(value)) == FAILURE)) {
        return;
    }

    ZVAL_OBJ(return_value, zend_objects_clone_obj(PROPERTY_ARG(getThis())));

    zend_update_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("uploadedFiles"), value);
}


/* parsedBody */

PHP_METHOD(ServerRequest, getParsedBody)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("parsedBody"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withParsedBody)
{
    zval *value = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_ARRAY_OR_OBJECT_EX(value, 1, 0);
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(PROPERTY_ARG(getThis())));

    if (EXPECTED(value != NULL)) {
        zend_update_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("parsedBody"), value);
    } else {
        zend_update_property_null(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("parsedBody"));
    }
}


/* attributes */

PHP_METHOD(ServerRequest, getAttributes)
{
    zval rv, *attributes;

    attributes = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("attributes"), 0, &rv);

    RETURN_ZVAL(attributes, 1, 0);
}

PHP_METHOD(ServerRequest, getAttribute)
{
    zval rv, *attributes, *value, *default_value = NULL;
    zend_string *name;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
        Z_PARAM_STR(name)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(default_value)
    ZEND_PARSE_PARAMETERS_END();

    attributes = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(getThis()), ZEND_STRL("attributes"), 0, &rv);

    value = zend_hash_find(Z_ARRVAL_P(attributes), name);

    /* value is only NULL if the entry wasn't found. A null value is still a zval. */
    if (value != NULL) {
        RETURN_ZVAL(value, 1, 0);
    } else if (default_value != NULL) {
        RETURN_ZVAL(default_value, 1, 0);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(ServerRequest, withAttribute)
{
    zval rv, *value, *attributes_prop;
    HashTable *attributes;
    zend_string *name;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_STR(name)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(PROPERTY_ARG(getThis())));

    attributes_prop = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("attributes"), 0, &rv);
    attributes = zend_array_dup(Z_ARR_P(attributes_prop));

    zend_symtable_update(attributes, name, value);
    ZVAL_ARR(attributes_prop, attributes);
}

PHP_METHOD(ServerRequest, withoutAttribute)
{
    zend_string *name;
    zval rv, *attributes_prop;
    HashTable *attributes;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR(name)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(PROPERTY_ARG(getThis())));

    attributes_prop = zend_read_property(HttpMessage_ServerRequest_ce, PROPERTY_ARG(return_value), ZEND_STRL("attributes"), 0, &rv);
    attributes = zend_array_dup(Z_ARR_P(attributes_prop));

    zend_symtable_del(attributes, name);
    ZVAL_ARR(attributes_prop, attributes);
}


/* Define HttpMessage\ServerRequest class */

static const zend_function_entry request_functions[] = {
    PHP_ME(ServerRequest, __construct, arginfo_HttpMessageServerRequest_construct, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(ServerRequest, getServerParams)
    HTTP_MESSAGE_ME(ServerRequest, getCookieParams)
    HTTP_MESSAGE_ME(ServerRequest, withCookieParams)
    HTTP_MESSAGE_ME(ServerRequest, getQueryParams)
    HTTP_MESSAGE_ME(ServerRequest, withQueryParams)
    HTTP_MESSAGE_ME(ServerRequest, getUploadedFiles)
    HTTP_MESSAGE_ME(ServerRequest, withUploadedFiles)
    HTTP_MESSAGE_ME(ServerRequest, getParsedBody)
    HTTP_MESSAGE_ME(ServerRequest, withParsedBody)
    HTTP_MESSAGE_ME(ServerRequest, getAttributes)
    HTTP_MESSAGE_ME(ServerRequest, getAttribute)
    HTTP_MESSAGE_ME(ServerRequest, withAttribute)
    HTTP_MESSAGE_ME(ServerRequest, withoutAttribute)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_serverrequest)
{
    zend_class_entry ce;
    zend_class_entry *interface = HTTP_MESSAGE_PSR_INTERFACE("serverrequest");

    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND(interface, "ServerRequest");
    if (HttpMessage_Request_ce == NULL) return FAILURE;

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "ServerRequest", request_functions);

    HttpMessage_ServerRequest_ce = zend_register_internal_class_ex(&ce, HttpMessage_Request_ce);
    zend_class_implements(HttpMessage_ServerRequest_ce, 1, interface);

    /* Properties */
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("serverParams"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("cookieParams"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("queryParams"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("uploadedFiles"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("parsedBody"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("attributes"), ZEND_ACC_PRIVATE);

    return SUCCESS;
}

#endif
