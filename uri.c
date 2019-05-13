/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - Uri class                               |
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
#include "ext/standard/info.h"
#include "ext/standard/url.h"
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Uri_ce;


/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUri_construct, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Uri, __construct)
{
    php_url *info;
    char *value;
    size_t value_len = 0;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END();

    if (value_len > 0) {
        info = php_url_parse_ex(value, value_len);

        SET_STRING_PROPERTY(HttpMessage_Uri_ce, "scheme", info->scheme);
        SET_STRING_PROPERTY(HttpMessage_Uri_ce, "host", info->host);
        SET_STRING_PROPERTY(HttpMessage_Uri_ce, "userInfo", info->user);
        SET_STRING_PROPERTY(HttpMessage_Uri_ce, "path", info->path);
        SET_STRING_PROPERTY(HttpMessage_Uri_ce, "query", info->query);
        SET_STRING_PROPERTY(HttpMessage_Uri_ce, "fragment", info->fragment);

        if (info->port > 0) {
            zend_update_property_long(HttpMessage_Uri_ce, getThis(), ZEND_STRL("port"), info->port);
        }
    }
}


/* __toString */


ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUri_toString, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Uri, __toString)
{
}


/* scheme */

PHP_METHOD(Uri, getScheme)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("schema"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withScheme)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("schema"), value, value_len);
}


/* authority */

PHP_METHOD(Uri, getAuthority)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("userInfo"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}


/* user info */

PHP_METHOD(Uri, getUserInfo)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("userInfo"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withUserInfo)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("userInfo"), value, value_len);
}


/* host */

PHP_METHOD(Uri, getHost)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("host"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withHost)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("host"), value, value_len);
}


/* port */

PHP_METHOD(Uri, getPort)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("port"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withPort)
{
    long value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(value)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_long(HttpMessage_Uri_ce, return_value, ZEND_STRL("port"), value);
}


/* path */

PHP_METHOD(Uri, getPath)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("path"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withPath)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("path"), value, value_len);
}


/* query */

PHP_METHOD(Uri, getQuery)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("query"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withQuery)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("query"), value, value_len);
}


/* query */

PHP_METHOD(Uri, getFragment)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("fragment"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withFragment)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("fragment"), value, value_len);
}


/* Define HttpMessage\Uri class */

static const zend_function_entry uri_functions[] = {
    PHP_ME(Uri, __construct, arginfo_HttpMessageUri_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Uri, __toString, arginfo_HttpMessageUri_toString, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(Uri, getScheme)
    HTTP_MESSAGE_ME(Uri, withScheme)
    HTTP_MESSAGE_ME(Uri, getAuthority)
    HTTP_MESSAGE_ME(Uri, getUserInfo)
    HTTP_MESSAGE_ME(Uri, withUserInfo)
    HTTP_MESSAGE_ME(Uri, getHost)
    HTTP_MESSAGE_ME(Uri, withHost)
    HTTP_MESSAGE_ME(Uri, getPort)
    HTTP_MESSAGE_ME(Uri, withPort)
    HTTP_MESSAGE_ME(Uri, getPath)
    HTTP_MESSAGE_ME(Uri, withPath)
    HTTP_MESSAGE_ME(Uri, getQuery)
    HTTP_MESSAGE_ME(Uri, withQuery)
    HTTP_MESSAGE_ME(Uri, getFragment)
    HTTP_MESSAGE_ME(Uri, withFragment)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_uri)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Uri", uri_functions);

    HttpMessage_Uri_ce = zend_register_internal_class(&ce);
    zend_class_implements(HttpMessage_Uri_ce, 1, PsrHttpMessageUriInterface_ce_ptr);

    /* Properties */
    zend_declare_property_string(HttpMessage_Uri_ce, ZEND_STRL("scheme"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Uri_ce, ZEND_STRL("userInfo"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Uri_ce, ZEND_STRL("host"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_Uri_ce, ZEND_STRL("port"), ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Uri_ce, ZEND_STRL("path"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Uri_ce, ZEND_STRL("query"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Uri_ce, ZEND_STRL("fragment"), "", ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
