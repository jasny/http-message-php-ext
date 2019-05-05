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

#include "php.h"
#include "php_ini.h"
#include "php_http_message.h"
#include "macros.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/standard/info.h"
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_ServerRequest_ce;


/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_ServerRequest_construct, 0, 0, 0)
        ZEND_ARG_TYPE_INFO(0, serverParams, IS_ARRAY, 0)
        ZEND_ARG_TYPE_INFO(0, cookieParams, IS_ARRAY, 0)
        ZEND_ARG_TYPE_INFO(0, queryParams, IS_ARRAY, 0)
        ZEND_ARG_INFO(0, parsedBody)
        ZEND_ARG_TYPE_INFO(0, files, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(ServerRequest, __construct)
{
    zval rv;

    /* parent::__construct() */
    zend_call_method_with_0_params(
            getThis(), HttpMessage_Request_ce, &HttpMessage_Request_ce->constructor, "__construct", NULL
    );

    INIT_ARRAY_PROPERTY("serverParams");
    INIT_ARRAY_PROPERTY("cookieParams");
    INIT_ARRAY_PROPERTY("queryParams");
    INIT_ARRAY_PROPERTY("uploadedFiles");
    INIT_ARRAY_PROPERTY("attributes");
}


/* serverParams */

PHP_METHOD(ServerRequest, getServerParams)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, getThis(), ZEND_STRL("serverParams"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}


/* cookieParams */

PHP_METHOD(ServerRequest, getCookieParams)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, getThis(), ZEND_STRL("cookieParams"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withCookieParams)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_ARRAY(value);
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_ServerRequest_ce, return_value, ZEND_STRL("cookieParams"), value);
}


/* queryParams */

PHP_METHOD(ServerRequest, getQueryParams)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, getThis(), ZEND_STRL("queryParams"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withQueryParams)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_ARRAY(value);
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_ServerRequest_ce, return_value, ZEND_STRL("queryParams"), value);
}


/* queryParams */

PHP_METHOD(ServerRequest, getUploadedFiles)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, getThis(), ZEND_STRL("uploadedFiles"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withUploadedFiles)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_ARRAY(value);
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_ServerRequest_ce, return_value, ZEND_STRL("uploadedFiles"), value);
}


/* parsedBody */

PHP_METHOD(ServerRequest, getParsedBody)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_ServerRequest_ce, getThis(), ZEND_STRL("parsedBody"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(ServerRequest, withParsedBody)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_ARRAY_OR_OBJECT(value, 0, 0);
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_ServerRequest_ce, return_value, ZEND_STRL("parsedBody"), value);
}


/* attributes */

PHP_METHOD(ServerRequest, getAttributes)
{
    zval rv, *attributes;

    attributes = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("attributes"), 0, &rv);

    RETURN_ZVAL(attributes, 1, 0);
}

PHP_METHOD(ServerRequest, getAttribute)
{
    zval rv, *attributes, *value, *default_value;
    char *name;
    size_t name_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_STRING(name, name_len)
            Z_PARAM_ZVAL(default_value)
    ZEND_PARSE_PARAMETERS_END_EX();

    attributes = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("attributes"), 0, &rv);

    value = zend_hash_str_find(Z_ARRVAL_P(attributes), name, name_len);

    /* value is only NULL if the entry wasn't found. A null value is still a zval. */
    RETURN_ZVAL(value != NULL ? value : default_value, 1, 0);
}

PHP_METHOD(ServerRequest, withAttribute)
{
    zval rv, *value, *attributes, new_attributes;
    char *name;
    size_t name_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
            Z_PARAM_STRING(name, name_len)
            Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END_EX();

    attributes = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("attributes"), 0, &rv);
    ZVAL_COPY(&new_attributes, attributes);

    add_assoc_zval_ex(&new_attributes, name, name_len, value);

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Message_ce, return_value, ZEND_STRL("attributes"), &new_attributes);
}

PHP_METHOD(ServerRequest, withoutAttribute)
{
    zval rv, *attributes;
    char *name;
    size_t name_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
            Z_PARAM_STRING(name, name_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    attributes = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("attributes"), 0, &rv);
    zend_hash_str_del(Z_ARRVAL_P(attributes), name, name_len);

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Message_ce, return_value, ZEND_STRL("attributes"), attributes);
}


/* Define HttpMessage\ServerRequest class */

static const zend_function_entry request_functions[] = {
        PHP_ME(ServerRequest, __construct, arginfo_ServerRequest_construct, ZEND_ACC_PUBLIC)
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
    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "ServerRequest", request_functions);

    HttpMessage_ServerRequest_ce = zend_register_internal_class_ex(&ce, HttpMessage_Request_ce);
    zend_class_implements(HttpMessage_ServerRequest_ce, 1, PsrHttpMessageServerRequestInterface_ce_ptr);

    /* Properties */
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("serverParams"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("cookieParams"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("queryParams"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("uploadedFiles"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("parsedBody"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_ServerRequest_ce, ZEND_STRL("attributes"), ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
