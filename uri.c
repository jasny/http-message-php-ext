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
#include "zend_smart_str.h"
#include "ext/standard/info.h"
#include "ext/standard/url.h"
#include "ext/psr/psr_http_message.h"
#include "ext/spl/spl_exceptions.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Uri_ce;

void uri_set_userinfo(zval *uri, char *user, size_t user_len, char *pass, size_t pass_len)
{
    char *userinfo;

    if (user_len == 0 && pass_len == 0) {
        return;
    }

    if (pass_len == 0) {
        zend_update_property_stringl(HttpMessage_Uri_ce, uri, ZEND_STRL("userInfo"), user, user_len);
    } else {
        userinfo = emalloc(user_len + pass_len + 2);
        if (UNEXPECTED(userinfo == NULL)) return; // Memory issue

        user[user_len] = '\0';
        pass[pass_len] = '\0';
        sprintf(userinfo, "%s:%s", user, pass);

        zend_update_property_stringl(HttpMessage_Uri_ce, uri, ZEND_STRL("userInfo"), userinfo, user_len + pass_len + 1);

        efree(userinfo);
    }
}

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

        if (info == NULL) {
            zend_throw_exception(spl_ce_UnexpectedValueException, "Invalid uri", 0);
            return;
        }

        SET_URI_PROPERTY(HttpMessage_Uri_ce, "scheme", info->scheme);
        SET_URI_PROPERTY(HttpMessage_Uri_ce, "host", info->host);
        SET_URI_PROPERTY(HttpMessage_Uri_ce, "path", info->path);
        SET_URI_PROPERTY(HttpMessage_Uri_ce, "query", info->query);
        SET_URI_PROPERTY(HttpMessage_Uri_ce, "fragment", info->fragment);

        if (info->port > 0) {
            zend_update_property_long(HttpMessage_Uri_ce, getThis(), ZEND_STRL("port"), info->port);
        }

#if PHP_VERSION_ID < 70300
        uri_set_userinfo(getThis(), info->user, STRLEN_NULL(info->user), info->pass, STRLEN_NULL(info->pass));
#else
        uri_set_userinfo(getThis(), ZSTR_VAL_LEN(info->user), ZSTR_VAL_LEN(info->pass));
#endif
    }
}


/* __toString */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUri_toString, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Uri, __toString)
{
    zval rv, *scheme, *userinfo, *host, *port, *path, *query, *fragment;
    smart_str buf = {0};
    char *path_ptr;
    size_t path_len = 0;

    scheme = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("scheme"), 0, &rv);
    userinfo = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("userInfo"), 0, &rv);
    host = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("host"), 0, &rv);
    port = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("port"), 0, &rv);
    path = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("path"), 0, &rv);
    query = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("query"), 0, &rv);
    fragment = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("fragment"), 0, &rv);

    smart_str_alloc(&buf, 0, 0);

    if (Z_STRLEN(*scheme) > 0) {
        smart_str_appendl(&buf, Z_STRVAL(*scheme), Z_STRLEN(*scheme));
        smart_str_appends(&buf, ":");
    }

    if (Z_STRLEN(*host) > 0) {
        smart_str_appends(&buf, "//");

        if (Z_STRLEN(*userinfo) > 0) {
            smart_str_appendl(&buf, Z_STRVAL(*userinfo), Z_STRLEN(*userinfo));
            smart_str_appends(&buf, "@");
        }

        smart_str_appendl(&buf, Z_STRVAL(*host), Z_STRLEN(*host));

        if (Z_TYPE(*port) == IS_LONG) {
            smart_str_appends(&buf, ":");
            smart_str_append_long(&buf, Z_LVAL(*port));
        }
    }

    if (Z_STRLEN(*path) > 0) {
        if (Z_STRLEN(*host) > 0 && *Z_STRVAL(*path) != '/') {
            smart_str_appends(&buf, "/");
        }

        path_ptr = Z_STRVAL(*path);
        path_len = Z_STRLEN(*path);

        if (Z_STRLEN(*host) == 0) {
            while (path_len > 1 && *path_ptr == '/' && *(path_ptr + 1) == '/') {
                path_ptr++;
                path_len--;
            }
        }

        smart_str_appendl(&buf, path_ptr, path_len);
    }

    if (Z_STRLEN(*query) > 0) {
        smart_str_appends(&buf, "?");
        smart_str_appendl(&buf, Z_STRVAL(*query), Z_STRLEN(*query));
    }

    if (Z_STRLEN(*fragment) > 0) {
        smart_str_appends(&buf, "#");
        smart_str_appendl(&buf, Z_STRVAL(*fragment), Z_STRLEN(*fragment));
    }

    RETVAL_STR_COPY(buf.s);
    zend_string_release(buf.s);
}


/* scheme */

PHP_METHOD(Uri, getScheme)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("scheme"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Uri, withScheme)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("scheme"), value, value_len);
}


/* authority */

PHP_METHOD(Uri, getAuthority)
{
    zval rv, *userinfo, *host, *port;
    smart_str buf = {0};

    userinfo = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("userInfo"), 0, &rv);
    host = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("host"), 0, &rv);
    port = zend_read_property(HttpMessage_Uri_ce, getThis(), ZEND_STRL("port"), 0, &rv);

    if (Z_STRLEN_P(host) == 0) {
        RETURN_EMPTY_STRING();
    }

    if (Z_STRLEN_P(userinfo) > 0) {
        smart_str_appendl(&buf, Z_STRVAL_P(userinfo), Z_STRLEN_P(userinfo));
        smart_str_appends(&buf, "@");
    }

    smart_str_appendl(&buf, Z_STRVAL_P(host), Z_STRLEN_P(host));

    if (Z_TYPE_P(port) == IS_LONG) {
        smart_str_appends(&buf, ":");
        smart_str_append_long(&buf, Z_LVAL_P(port));
    }

    RETVAL_STR_COPY(buf.s);
    zend_string_release(buf.s);
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
    ZEND_PARSE_PARAMETERS_END();

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
    ZEND_PARSE_PARAMETERS_END();

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
    zend_bool is_null;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG_EX(value, is_null, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    if (is_null) {
        zend_update_property_null(HttpMessage_Uri_ce, return_value, ZEND_STRL("port"));
    } else {
        zend_update_property_long(HttpMessage_Uri_ce, return_value, ZEND_STRL("port"), value);
    }
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
    ZEND_PARSE_PARAMETERS_END();

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
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, ZEND_STRL("query"), value, value_len);
}


/* fragment */

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
    ZEND_PARSE_PARAMETERS_END();

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
