/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - Response class                          |
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
#include "ext/psr/psr_http_message.h"
#include "ext/spl/spl_exceptions.h"
#include "php_http_message.h"
#include "macros.h"
#include "response.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Response_ce;

int response_set_status(zval *obj, zend_long code, zend_string *phrase)
{
    const char *suggested_phrase;

    if (code < 100 || code > 999) {
        zend_throw_exception(spl_ce_InvalidArgumentException, "Invalid HTTP status code %ld", code);
        return FAILURE;
    }

    zend_update_property_long(HttpMessage_Response_ce, obj, ZEND_STRL("statusCode"), code);

    if (phrase != NULL) {
        zend_update_property_str(HttpMessage_Response_ce, obj, ZEND_STRL("reasonPhrase"), phrase);
    } else {
        suggested_phrase = get_status_string((int)code);
        zend_update_property_stringl(
                HttpMessage_Response_ce, obj, ZEND_STRL("reasonPhrase"),
                suggested_phrase, strlen(suggested_phrase)
        );
    }

    return SUCCESS;
}

/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageServerRequest_construct, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, statusCode, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, reasonPhrase, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Response, __construct)
{
    zend_long code = 0;
    zend_string *phrase = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(code);
        Z_PARAM_STR_EX(phrase, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    if (code > 0) {
        response_set_status(getThis(), code, phrase);
    }
}


/* status */

PHP_METHOD(Response, getStatusCode)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Response_ce, getThis(), ZEND_STRL("statusCode"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Response, getReasonPhrase)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Response_ce, getThis(), ZEND_STRL("reasonPhrase"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Response, withStatus)
{
    zend_long code = 0;
    zend_string *phrase = NULL;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
        Z_PARAM_LONG(code)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR_EX(phrase, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    response_set_status(return_value, code, phrase);
}


/* Define HttpMessage\Response class */

static const zend_function_entry response_functions[] = {
    HTTP_MESSAGE_ME(Response, getStatusCode)
    HTTP_MESSAGE_ME(Response, getReasonPhrase)
    HTTP_MESSAGE_ME(Response, withStatus)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_response)
{
    zend_class_entry ce;
    zend_class_entry *interface = HTTP_MESSAGE_PSR_INTERFACE("response");

    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND(interface, "Response");
    if (HttpMessage_Message_ce == NULL) return FAILURE;

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Response", response_functions);

    HttpMessage_Response_ce = zend_register_internal_class_ex(&ce, HttpMessage_Message_ce);
    zend_class_implements(HttpMessage_Response_ce, 1, interface);

    /* Properties */
    zend_declare_property_long(HttpMessage_Response_ce, ZEND_STRL("statusCode"), 0, ZEND_ACC_PRIVATE);
    zend_declare_property_string(HttpMessage_Response_ce, ZEND_STRL("reasonPhrase"), "", ZEND_ACC_PRIVATE);

    return SUCCESS;
}

#endif
