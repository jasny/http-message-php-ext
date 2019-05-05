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
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Uri_ce;


/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUri_construct, 0, 0, 1)
        ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Uri, __construct)
{
}


/* scheme */

PHP_METHOD(Uri, getScheme)
{
    zval rv, *value;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    value = zend_read_property(HttpMessage_Uri_ce, getThis(), "schema", sizeof("schema") - 1, 0, &rv);

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

    zend_update_property_stringl(HttpMessage_Uri_ce, return_value, "schema", sizeof("schema") - 1, value, value_len);
}


/* Define HttpMessage\Uri class */

static const zend_function_entry uri_functions[] = {
    PHP_ME(Uri, __construct, arginfo_HttpMessageUri_construct, ZEND_ACC_PROTECTED)
    HTTP_MESSAGE_ME(Uri, getScheme)
    HTTP_MESSAGE_ME(Uri, withScheme)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_uri)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Uri", uri_functions);

    HttpMessage_Uri_ce = zend_register_internal_class(&ce);
    /*zend_class_implements(HttpMessage_Uri_ce, 1, PsrHttpMessageUriInterface_ce_ptr);*/

    /* Properties */
    zend_declare_property_string(HttpMessage_Uri_ce, "scheme", sizeof("scheme") - 1, "", ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
