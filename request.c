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
#include "request.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_set, 0, 0, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()


zend_class_entry *request_ce;

/*
HTTP_MESSAGE_GET_METHOD(Request, getRequestTarget, request_ce, "requestTarget")
*/

PHP_METHOD(Request, getRequestTarget)
{
    zval rv, *obj, *value;

    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    obj = getThis();

    value = zend_read_property(request_ce, obj, "requestTarget", sizeof("requestTarget") - 1, 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Request, withRequestTarget)
{
    zval *obj, *new_value;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &new_value) == FAILURE) {
        return;
    }

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(request_ce, return_value, "requestTarget", sizeof("requestTarget") - 1, new_value);
}

/* Define HttpMessage\Request class */

static const zend_function_entry request_functions[] = {
    PHP_ME(Request, getRequestTarget, arginfo_void, ZEND_ACC_PUBLIC)
    PHP_ME(Request, withRequestTarget, arginfo_set, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_request)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "HttpMessage\\Request", request_functions);

    request_ce = zend_register_internal_class(&ce);
    /*zend_class_implements(request_ce, 1, PsrHttpMessageRequestInterface_ce_ptr);*/

    /* Properties */
    zend_declare_property_string(
        request_ce, "requestTarget", sizeof("requestTarget") - 1, "/", ZEND_ACC_PUBLIC
    );

    return SUCCESS;
}

#endif
