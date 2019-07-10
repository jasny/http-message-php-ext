/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - Request class                           |
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
#include "zend_smart_str.h"
#include "ext/standard/info.h"
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Request_ce = NULL;


/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_Request_construct, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Request, __construct)
{
    zval rv, *uri;

    /* parent::__construct() */
    zend_call_method_with_0_params(
            getThis(), HttpMessage_Message_ce, &HttpMessage_Message_ce->constructor, "__construct", NULL
    );

    /* $this->uri = new Uri() */
    uri = zend_read_property(HttpMessage_Request_ce, getThis(), ZEND_STRL("uri"), 0, &rv);
    object_init_ex(uri, HttpMessage_Uri_ce);
}


/* requestTarget */

PHP_METHOD(Request, getRequestTarget)
{
    zval rv, *value, *uri, path, query;
    smart_str buf = {0};

    value = zend_read_property(HttpMessage_Request_ce, getThis(), ZEND_STRL("requestTarget"), 0, &rv);

    if (!ZVAL_IS_NULL(value)) {
        RETURN_ZVAL(value, 1, 0);
    }

    uri = zend_read_property(HttpMessage_Request_ce, getThis(), ZEND_STRL("uri"), 0, &rv);
    zend_call_method_with_0_params(uri, NULL, NULL, "getPath", &path);
    zend_call_method_with_0_params(uri, NULL, NULL, "getQuery", &query);

    if (UNEXPECTED(Z_TYPE(path) != IS_STRING) || Z_STRLEN(path) == 0) {
        RETURN_STRING("/");
    }

    if (Z_TYPE(query) != IS_STRING || Z_STRLEN(query) == 0) {
        RETURN_ZVAL(&path, 1, 0);
    }

    smart_str_appendl(&buf, Z_STRVAL(path), Z_STRLEN(path));
    smart_str_appends(&buf, "?");
    smart_str_appendl(&buf, Z_STRVAL(query), Z_STRLEN(query));

    RETVAL_STR_COPY(buf.s);
    zend_string_release(buf.s);
}

PHP_METHOD(Request, withRequestTarget)
{
    zend_string *value = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STR_EX(value, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    if (value != NULL) {
        zend_update_property_str(HttpMessage_Request_ce, return_value, ZEND_STRL("requestTarget"), value);
    } else {
        zend_update_property_null(HttpMessage_Request_ce, return_value, ZEND_STRL("requestTarget"));
    }
}


/* method */

PHP_METHOD(Request, getMethod)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Request_ce, getThis(), ZEND_STRL("method"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Request, withMethod)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(
            HttpMessage_Request_ce, return_value, ZEND_STRL("method"), value, value_len
    );
}


/* uri */

PHP_METHOD(Request, getUri)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Request_ce, getThis(), ZEND_STRL("uri"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Request, withUri)
{
    zval *value;
    zend_class_entry *uri_interface = get_internal_ce(ZEND_STRL("psr\\http\\message\\uriinterface"));

    if (uri_interface == NULL) {
        zend_throw_error(NULL, "Psr\\Http\\Message\\UriInterface not foud");
        return;
    }

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(value, uri_interface)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Request_ce, return_value, ZEND_STRL("uri"), value);
}


/* Define HttpMessage\Request class */

static const zend_function_entry request_functions[] = {
    PHP_ME(Request, __construct, arginfo_none, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(Request, getRequestTarget)
    HTTP_MESSAGE_ME(Request, withRequestTarget)
    HTTP_MESSAGE_ME(Request, getMethod)
    HTTP_MESSAGE_ME(Request, withMethod)
    HTTP_MESSAGE_ME(Request, getUri)
    HTTP_MESSAGE_ME(Request, withUri)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_request)
{
    zend_class_entry ce;
    zend_class_entry *interface = get_internal_ce(ZEND_STRL("psr\\http\\message\\requestinterface"));

    if (interface == NULL) return FAILURE;
    if (HttpMessage_Message_ce == NULL) return FAILURE;

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Request", request_functions);

    HttpMessage_Request_ce = zend_register_internal_class_ex(&ce, HttpMessage_Message_ce);
    zend_class_implements(HttpMessage_Request_ce, 1, interface);

    /* Properties */
    zend_declare_property_null(HttpMessage_Request_ce, ZEND_STRL("requestTarget"), ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Request_ce, ZEND_STRL("method"), "", ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_Request_ce, ZEND_STRL("uri"), ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
