/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - Stream class                            |
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

#include "SAPI.h"
#include "php.h"
#include "macros.h"
#include "response.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/spl/spl_exceptions.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Emitter_ce = NULL;

int assert_no_headers_sent()
{
    if (!SG(headers_sent)) {
        return SUCCESS;
    }

    const char *file = php_output_get_start_filename();
    int line = php_output_get_start_lineno();

    if (file != NULL) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0,
                "Cannot send session cookie - headers already sent by (output started at %s:%d)", file, line);
    } else {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Cannot send session cookie - headers already sent");
    }

    return FAILURE;
}

int read_response_body(zval *response, zval *contents)
{
    zval body;

    ZVAL_NULL(&body);
    ZVAL_NULL(contents);

    zend_call_method_with_0_params(PROPERTY_ARG(response), NULL, NULL, "getBody", &body);

    if (EXPECTED(Z_TYPE(body) == IS_OBJECT)) {
        zend_call_method_with_0_params(PROPERTY_ARG(&body), NULL, NULL, "__toString", contents);
    }

    if (UNEXPECTED(Z_TYPE_P(contents) != IS_STRING)) {
        return FAILURE; // Something went wrong, assuming an error was already thrown.
    }

    return SUCCESS;
}

void emit_status(zval *response)
{
    zval version, status_code, reason_phrase;
    zend_long code;
    char *phrase = NULL;
    size_t phrase_len = 0;
    sapi_header_line ctr = {NULL, 0, 0};

    ZVAL_NULL(&status_code);
    ZVAL_NULL(&reason_phrase);

    zend_call_method_with_0_params(PROPERTY_ARG(response), NULL, NULL, "getProtocolVersion", &version);
    zend_call_method_with_0_params(PROPERTY_ARG(response), NULL, NULL, "getStatusCode", &status_code);
    zend_call_method_with_0_params(PROPERTY_ARG(response), NULL, NULL, "getReasonPhrase", &reason_phrase);
    
    code = Z_LVAL(status_code);

    if (Z_STRLEN(reason_phrase) > 0) {
        phrase = Z_STRVAL(reason_phrase);
        phrase_len = Z_STRLEN(reason_phrase);
    } else {
        phrase = (char *)get_status_string((int)code);
        phrase_len = strlen(phrase);
    }

    ctr.line_len = Z_STRLEN(version) + phrase_len + 10;
    ctr.line = emalloc(ctr.line_len);
    zend_sprintf((char *)ctr.line, "HTTP/%.*s %3lu %.*s", (int)Z_STRLEN(version), Z_STRVAL(version), code, (int)phrase_len, phrase);
    ctr.response_code = code;

    sapi_header_op(SAPI_HEADER_REPLACE, &ctr);

    efree((char *)ctr.line);
}

void emit_header(zend_string *header_name, zend_array *header_lines)
{
    zval *header_line;
    size_t max_header_size = 256;
    sapi_header_line ctr = {NULL, 0, 0};
    ctr.line = emalloc(256);

    ZEND_HASH_FOREACH_VAL(header_lines, header_line) {
        ctr.line_len = ZSTR_LEN(header_name) + Z_STRLEN_P(header_line) + 2;

        // Reuse ctr: resize if needed.
        if (ctr.line_len >= max_header_size) {
            efree((char *)ctr.line);
            max_header_size = (ctr.line_len + 255) - ((ctr.line_len + 255) % 256);
            ctr.line = emalloc(max_header_size);
        }

        zend_sprintf((char *)ctr.line, "%s: %s", ZSTR_VAL(header_name), Z_STRVAL_P(header_line));
        sapi_header_op(SAPI_HEADER_ADD, &ctr);
    } ZEND_HASH_FOREACH_END();

    efree((char *)ctr.line);
}

void emit_headers(zval *response)
{
    zval headers;
    zval *header_lines;
    zend_string *header_name;
    zend_long index;

    zend_call_method_with_0_params(PROPERTY_ARG(response), NULL, NULL, "getHeaders", &headers);

    ZEND_HASH_FOREACH_KEY_VAL(Z_ARR(headers), index, header_name, header_lines) {
        if (UNEXPECTED(header_name == NULL)) {
            zend_error(E_WARNING, "Unexpected response header key '%ld': header names should not be numeric", index);
            continue;
        }

        emit_header(header_name, Z_ARR_P(header_lines));
    } ZEND_HASH_FOREACH_END();
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageEmitter_emit, 0, 0, 0)
    ZEND_ARG_OBJ_INFO(0, serverParams, Psr\\Http\\Message\\ResponseInterface, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Emitter, emit)
{
    zval *response = NULL;
    zval contents;

    zend_class_entry *response_interface = HTTP_MESSAGE_PSR_INTERFACE("response");

    if (UNEXPECTED(response_interface == NULL)) {
        zend_throw_error(NULL, "Psr\\Http\\Message\\ResponseInterface not found");
        return;
    }

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_OBJECT_OF_CLASS(response, response_interface)
    ZEND_PARSE_PARAMETERS_END();

    if (UNEXPECTED(assert_no_headers_sent() == FAILURE)) {
        return;
    }

    if (UNEXPECTED(read_response_body(response, &contents) == FAILURE)) {
        return;
    }

    emit_headers(response);
    emit_status(response);
    zend_write(Z_STRVAL(contents), Z_STRLEN(contents));
}

/* Define HttpMessage\Emitter class */

static const zend_function_entry methods[] = {
    PHP_ME(Emitter, emit, arginfo_HttpMessageEmitter_emit, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_emitter)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Emitter", methods);
    HttpMessage_Emitter_ce = zend_register_internal_class(&ce);

    return SUCCESS;
}

#endif
