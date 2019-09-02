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

#include "php.h"
#include "macros.h"
#include "stream.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/psr/psr_http_message.h"
#include "ext/spl/spl_exceptions.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_Stream_ce = NULL;

void stream_seek(zval *this, zend_long offset, zend_long whence, zval* return_value)
{
    zval rv, *resource;
    php_stream *stream;

    resource = zend_read_property(HttpMessage_Stream_ce, this, ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream is %s",
                                Z_TYPE_P(resource) == IS_RESOURCE ? "closed" : "detached");
        return;
    }

    if (whence > 3) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Invalid value for whence");
        return;
    }

    php_stream_from_zval(stream, resource);

    if (!stream->ops->seek || (stream->flags & PHP_STREAM_FLAG_NO_SEEK) != 0) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream is not seekable");
        return;
    };

    php_stream_seek(stream, offset, whence);
}

/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageStream_construct, 0, 0, 0)
    ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

PHP_METHOD(Stream, __construct)
{
    zval *input = NULL, resource;
    char *file = NULL, *mode = NULL;
    size_t mode_len = 0;
    php_stream *stream;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(input)
        Z_PARAM_STRING(mode, mode_len)
    ZEND_PARSE_PARAMETERS_END();

    if (UNEXPECTED(input != NULL && Z_TYPE_P(input) != IS_STRING && Z_TYPE_P(input) != IS_RESOURCE)) {
        zend_type_error("Expected parameter 1 to be a string or resource, %s given ", zend_zval_type_name(input));
        return;
    }

    if (input == NULL) {
        if (open_temp_stream(&resource) == FAILURE) return;
    } else if (Z_TYPE_P(input) == IS_STRING) {
        file = Z_STRVAL_P(input);
        file[Z_STRLEN_P(input)] = '\0';
        stream = php_stream_open_wrapper(file, mode != NULL ? mode : "r", 0, NULL);

        if (stream == NULL) {
            zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to open '%s' stream", file);
            return;
        }

        php_stream_to_zval(stream, &resource);
    } else if (EXPECTED(IS_STREAM_RESOURCE(input))) {
        ZVAL_COPY(&resource, input);
    } else {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Resource is not a stream");
        return;
    }

    zend_update_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), &resource);
}

PHP_METHOD(Stream, __toString)
{
    zval rv, *resource;
    php_stream *stream;
    zend_string *contents;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        RETURN_EMPTY_STRING();
    }

    php_stream_from_zval(stream, resource);

    if (!string_contains_char(stream->mode, 'r') && !string_contains_char(stream->mode, '+')) {
        RETURN_EMPTY_STRING();
    }

    if ((stream->ops->seek) && (stream->flags & PHP_STREAM_FLAG_NO_SEEK) == 0) {
        php_stream_seek(stream, 0, SEEK_SET);
    }

    // Special case for 'php://input'. Need to reopen the resource, because seek doesn't work.
    if (
        stream->wrapper != NULL &&
        strcmp(stream->wrapper->wops->label, "PHP") == 0 &&
        strcmp(stream->ops->label, "Input") == 0
    ) {
        stream = php_stream_open_wrapper(stream->orig_path, stream->mode, 0, NULL);
        php_stream_to_zval(stream, resource);
    }

    if ((contents = php_stream_copy_to_mem(stream, (ssize_t)PHP_STREAM_COPY_ALL, 0))) {
        RETURN_STR(contents);
    } else {
        RETURN_EMPTY_STRING();
    }
}

PHP_METHOD(Stream, close)
{
    zval rv, *resource;
    php_stream *stream;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (Z_TYPE_P(resource) != IS_RESOURCE) {
        return;
    }

    if (IS_STREAM_RESOURCE(resource)) {
        php_stream_from_zval(stream, resource);
        php_stream_close(stream);
    }
}

PHP_METHOD(Stream, detach)
{
    zval rv, *resource;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    ZVAL_COPY(return_value, resource);

    zend_update_property_null(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"));
}

PHP_METHOD(Stream, getSize)
{
    zval rv, *resource;
    php_stream *stream;
    php_stream_statbuf ssb;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        RETURN_NULL();
    }

    php_stream_from_zval(stream, resource);
    php_stream_stat(stream, &ssb);

    RETURN_LONG(ssb.sb.st_size);
}

PHP_METHOD(Stream, tell)
{
    zval rv, *resource;
    php_stream *stream;
    size_t pos;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream is %s",
                Z_TYPE_P(resource) == IS_RESOURCE ? "closed" : "detached");
        return;
    }

    php_stream_from_zval(stream, resource);
    pos = php_stream_tell(stream);

    RETURN_LONG(pos);
}

PHP_METHOD(Stream, eof)
{
    zval rv, *resource;
    php_stream *stream;
    zend_bool eof;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        RETURN_TRUE;
    }

    php_stream_from_zval(stream, resource);
    eof = php_stream_eof(stream);

    RETURN_BOOL(eof);
}

PHP_METHOD(Stream, isSeekable)
{
    zval rv, *resource;
    php_stream *stream;
    zend_bool seekable;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        RETURN_FALSE;
    }

    php_stream_from_zval(stream, resource);
    seekable = (stream->ops->seek) && (stream->flags & PHP_STREAM_FLAG_NO_SEEK) == 0;

    RETURN_BOOL(seekable);
}

PHP_METHOD(Stream, seek)
{
    zend_long offset = 0, whence = SEEK_SET;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 2)
        Z_PARAM_LONG(offset)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(whence)
    ZEND_PARSE_PARAMETERS_END();

    stream_seek(getThis(), offset, whence, return_value);
}

PHP_METHOD(Stream, rewind)
{
    stream_seek(getThis(), 0, SEEK_SET, return_value);
}

PHP_METHOD(Stream, isWritable)
{
    zval rv, *resource;
    php_stream *stream;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        RETURN_FALSE;
    }

    php_stream_from_zval(stream, resource);

    RETURN_BOOL(stream_is_writable(stream));
}

PHP_METHOD(Stream, write)
{
    zval rv, *resource;
    char *input = NULL;
    size_t len = 0, ret = 0;
    php_stream *stream;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_STRING(input, len)
    ZEND_PARSE_PARAMETERS_END();

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream is %s",
                Z_TYPE_P(resource) == IS_RESOURCE ? "closed" : "detached");
        return;
    }

    php_stream_from_zval(stream, resource);

    if (!stream_is_writable(stream)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream not writable");
        return;
    }

    if (len > 0) {
        ret = php_stream_write(stream, input, len);
    }

    RETURN_LONG(ret);
}

PHP_METHOD(Stream, isReadable)
{
    zval rv, *resource;
    php_stream *stream;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        RETURN_FALSE;
    }

    php_stream_from_zval(stream, resource);

    RETURN_BOOL(stream_is_readable(stream));
}

PHP_METHOD(Stream, read)
{
    zval rv, *resource;
    php_stream *stream;
    zend_long len = 0;
    zend_string *contents;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 1)
        Z_PARAM_LONG(len)
    ZEND_PARSE_PARAMETERS_END();

    if (len < 0) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Length parameter must be equal or greater than 0");
        return;
    }

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream is %s",
                Z_TYPE_P(resource) == IS_RESOURCE ? "closed" : "detached");
        return;
    }

    php_stream_from_zval(stream, resource);

    if (!string_contains_char(stream->mode, 'r') && !string_contains_char(stream->mode, '+')) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream not readable");
        return;
    }

    if ((contents = php_stream_copy_to_mem(stream, (size_t)len, 0))) {
        RETURN_STR(contents);
    } else {
        RETURN_EMPTY_STRING();
    }
}

PHP_METHOD(Stream, getContents)
{
    zval rv, *resource;
    php_stream *stream;
    zend_string *contents;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    if (!IS_STREAM_RESOURCE(resource)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream is %s",
            Z_TYPE_P(resource) == IS_RESOURCE ? "closed" : "detached");
        return;
    }

    php_stream_from_zval(stream, resource);

    if (!string_contains_char(stream->mode, 'r') && !string_contains_char(stream->mode, '+')) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Stream not readable");
        return;
    }

    if ((contents = php_stream_copy_to_mem(stream, (size_t)PHP_STREAM_COPY_ALL, 0))) {
        RETURN_STR(contents);
    } else {
        RETURN_EMPTY_STRING();
    }
}

PHP_METHOD(Stream, getMetadata)
{
    zval rv, fname, *resource, *zvalue;
    zend_string *key = NULL;

    resource = zend_read_property(HttpMessage_Stream_ce, getThis(), ZEND_STRL("stream"), 0, &rv);
    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR(key)
    ZEND_PARSE_PARAMETERS_END();

    if (!IS_STREAM_RESOURCE(resource)) {
        if (key == NULL) {
            array_init(return_value);
        } else {
            RETVAL_NULL();
        }
        return;
    }

    ZVAL_STRING(&fname, "stream_get_meta_data");
    call_user_function(NULL, NULL, &fname, return_value, 1, resource);

    if (key != NULL) {
        zvalue = zend_hash_find(Z_ARR(*return_value), key);

        if (zvalue == NULL) {
            ZVAL_NULL(return_value);
        } else {
            ZVAL_COPY(return_value, zvalue);
        }
    }
}


/* Define HttpMessage\Stream class */

static const zend_function_entry stream_functions[] = {
    PHP_ME(Stream, __construct, arginfo_HttpMessageStream_construct, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(Stream, __toString)
    HTTP_MESSAGE_ME(Stream, close)
    HTTP_MESSAGE_ME(Stream, detach)
    HTTP_MESSAGE_ME(Stream, getSize)
    HTTP_MESSAGE_ME(Stream, tell)
    HTTP_MESSAGE_ME(Stream, eof)
    HTTP_MESSAGE_ME(Stream, isSeekable)
    HTTP_MESSAGE_ME(Stream, seek)
    HTTP_MESSAGE_ME(Stream, rewind)
    HTTP_MESSAGE_ME(Stream, isWritable)
    HTTP_MESSAGE_ME(Stream, write)
    HTTP_MESSAGE_ME(Stream, isReadable)
    HTTP_MESSAGE_ME(Stream, read)
    HTTP_MESSAGE_ME(Stream, getContents)
    HTTP_MESSAGE_ME(Stream, getMetadata)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_stream)
{
    zend_class_entry ce;
    zend_class_entry *interface = HTTP_MESSAGE_PSR_INTERFACE("stream");

    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND(interface, "Stream");

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "Stream", stream_functions);

    HttpMessage_Stream_ce = zend_register_internal_class(&ce);
    zend_class_implements(HttpMessage_Stream_ce, 1, interface);

    /* Properties */
    zend_declare_property_null(HttpMessage_Stream_ce, ZEND_STRL("stream"), ZEND_ACC_PRIVATE);

    return SUCCESS;
}

#endif
