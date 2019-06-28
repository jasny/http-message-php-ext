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
#include "php_ini.h"
#include "php_http_message.h"
#include "macros.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "http_status_codes.h"
#include "ext/standard/info.h"
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Response_ce;


/* Helper function to get reason phrase from status code */

static int status_comp(const void *a, const void *b)
{
    const http_response_status_code_pair *pa = (const http_response_status_code_pair *) a;
    const http_response_status_code_pair *pb = (const http_response_status_code_pair *) b;

    if (pa->code < pb->code) {
        return -1;
    } else if (pa->code > pb->code) {
        return 1;
    }

    return 0;
}

static const char *get_status_string(int code)
{
    http_response_status_code_pair needle = {code, NULL}, *result = NULL;

    result = bsearch(&needle, http_status_map, http_status_map_len, sizeof(needle), status_comp);

    return result ? result->str : "";
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
    zend_long code;
    char *phrase;
    size_t phrase_len = 0;
    const char *suggested_phrase;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
        Z_PARAM_LONG(code)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(phrase, phrase_len)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_long(HttpMessage_Response_ce, return_value, ZEND_STRL("statusCode"), code);

    if (phrase_len > 0) {
        zend_update_property_stringl(
                HttpMessage_Response_ce, return_value, ZEND_STRL("reasonPhrase"), phrase, phrase_len
        );
    } else {
        suggested_phrase = get_status_string((int)code);
        zend_update_property_stringl(
                HttpMessage_Response_ce, return_value, ZEND_STRL("reasonPhrase"),
                suggested_phrase, strlen(suggested_phrase)
        );
    }
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
    zend_class_entry *interface = get_internal_ce(ZEND_STRL("psr\\http\\message\\responseinterface"));

    if (interface == NULL) return FAILURE;
    if (HttpMessage_Message_ce == NULL) return FAILURE;

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Response", response_functions);

    HttpMessage_Response_ce = zend_register_internal_class_ex(&ce, HttpMessage_Message_ce);
    zend_class_implements(HttpMessage_Response_ce, 1, interface);

    /* Properties */
    zend_declare_property_long(HttpMessage_Response_ce, ZEND_STRL("statusCode"), 0, ZEND_ACC_PROTECTED);
    zend_declare_property_string(HttpMessage_Response_ce, ZEND_STRL("reasonPhrase"), "", ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
