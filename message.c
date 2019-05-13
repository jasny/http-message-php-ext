/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - Message methods                         |
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
#include "zend_string.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/psr/psr_http_message.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Message_ce;


/* __construct */

PHP_METHOD(Message, __construct)
{
    zval rv, *body;

    /* $this->body = new Stream() */
    body = zend_read_property(HttpMessage_Request_ce, getThis(), ZEND_STRL("body"), 0, &rv);
    object_init_ex(body, HttpMessage_Stream_ce);
    if (body != NULL && Z_TYPE_P(body) == IS_OBJECT) { /* Should always be true */
        zend_call_method_with_0_params(
                body, HttpMessage_Stream_ce, &HttpMessage_Stream_ce->constructor, "__construct", NULL
        );
    }

    INIT_ARRAY_PROPERTY(HttpMessage_Message_ce, "headers", rv);
}


/* protocolVersion */

PHP_METHOD(Message, getProtocolVersion)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("protocolVersion"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Message, withProtocolVersion)
{
    char *value;
    size_t value_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(value, value_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property_stringl(
            HttpMessage_Message_ce, return_value, ZEND_STRL("protocolVersion"), value, value_len
    );
}


/* headers */

PHP_METHOD(Message, getHeaders)
{
    zval rv, *headers;

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);

    RETURN_ZVAL(headers, 1, 0);
}

PHP_METHOD(Message, hasHeader)
{
    zval rv, *headers;
    char *name;
    size_t name_len;
    zend_bool exists;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(name, name_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);

    exists = zend_hash_str_exists(Z_ARRVAL_P(headers), name, name_len);

    RETVAL_BOOL(exists);
}

PHP_METHOD(Message, getHeader)
{
    zval rv, *headers, *header_values;
    char *name;
    size_t name_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(name, name_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);

    header_values = zend_hash_str_find(Z_ARRVAL_P(headers), name, name_len);
    if (header_values != NULL) {
        array_init(header_values);
    }

    RETURN_ZVAL(header_values, 1, 0);
}

PHP_METHOD(Message, getHeaderLine)
{
    zval rv, *headers, *header_values, *header_string;
    char *name;
    size_t name_len;
    zend_string *glue;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(name, name_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);
    header_values = zend_hash_str_find(Z_ARRVAL_P(headers), name, name_len);

    glue = zend_string_init(ZEND_STRL(", "), 0);
    php_implode(glue, header_values, header_string);

    zend_string_free(glue);

    RETURN_ZVAL(header_string, 0, 0)
}

PHP_METHOD(Message, withHeader)
{
    zval rv, *headers, new_headers, header_values;
    char *name;
    size_t name_len;
    zend_string *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_STRING(name, name_len)
        Z_PARAM_STR(value)
    ZEND_PARSE_PARAMETERS_END_EX();

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);
    ZVAL_COPY(&new_headers, headers);

    array_init(&header_values);
    add_next_index_str(&header_values, value);

    add_assoc_zval_ex(&new_headers, name, name_len, &header_values);

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Message_ce, return_value, ZEND_STRL("headers"), &new_headers);
}

PHP_METHOD(Message, withAddedHeader)
{
    zval rv, *headers, *header_values;
    char *name;
    size_t name_len;
    zend_string *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 2, 2)
        Z_PARAM_STRING(name, name_len)
        Z_PARAM_STR(value)
    ZEND_PARSE_PARAMETERS_END_EX();

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);

    header_values = zend_hash_str_find(Z_ARRVAL_P(headers), name, name_len);
    if (header_values == NULL) {
        array_init(header_values);
    }
    add_next_index_str(header_values, value);

    add_assoc_zval_ex(headers, name, name_len, header_values);

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Message_ce, return_value, ZEND_STRL("headers"), headers);
}

PHP_METHOD(Message, withoutHeader)
{
    zval rv, *headers;
    char *name;
    size_t name_len;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(name, name_len)
    ZEND_PARSE_PARAMETERS_END_EX();

    headers = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("headers"), 0, &rv);
    zend_hash_str_del(Z_ARRVAL_P(headers), name, name_len);

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Message_ce, return_value, ZEND_STRL("headers"), headers);
}


/* body */

PHP_METHOD(Message, getBody)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("body"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(Message, withBody)
{
    zval *value;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(value, PsrHttpMessageStreamInterface_ce_ptr)
    ZEND_PARSE_PARAMETERS_END_EX();

    ZVAL_OBJ(return_value, zend_objects_clone_obj(getThis()));

    zend_update_property(HttpMessage_Message_ce, return_value, ZEND_STRL("body"), value);
}


/* Define HttpMessage\Message class */

static const zend_function_entry message_functions[] = {
    PHP_ME(Message, __construct, arginfo_none, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(Message, getProtocolVersion)
    HTTP_MESSAGE_ME(Message, withProtocolVersion)
    HTTP_MESSAGE_ME(Message, getHeaders)
    HTTP_MESSAGE_ME(Message, hasHeader)
    HTTP_MESSAGE_ME(Message, getHeader)
    HTTP_MESSAGE_ME(Message, getHeaderLine)
    HTTP_MESSAGE_ME(Message, withHeader)
    HTTP_MESSAGE_ME(Message, withAddedHeader)
    HTTP_MESSAGE_ME(Message, withoutHeader)
    HTTP_MESSAGE_ME(Message, getBody)
    HTTP_MESSAGE_ME(Message, withBody)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_message)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Message", message_functions);

    HttpMessage_Message_ce = zend_register_internal_class(&ce);
    HttpMessage_Message_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    zend_class_implements(HttpMessage_Message_ce, 1, PsrHttpMessageMessageInterface_ce_ptr);

    /* Properties */
    zend_declare_property_string(
            HttpMessage_Message_ce, ZEND_STRL("protocolVersion"), "1.1", ZEND_ACC_PROTECTED
    );
    zend_declare_property_null(HttpMessage_Message_ce, ZEND_STRL("headers"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_Message_ce, ZEND_STRL("body"), ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
