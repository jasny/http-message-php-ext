/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension                                           |
  | Helper function to work with streams                                 |
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

#ifndef HTTP_MESSAGE_STREAM_H
#define HTTP_MESSAGE_STREAM_H

#include "php_streams.h"

static int open_temp_stream(zval *zstream)
{
    php_stream *stream = php_stream_open_wrapper("php://temp", "w+", 0, NULL);

    if (stream == NULL) {
        zend_throw_error(NULL, "Failed to open 'php://temp' stream");
        return FAILURE;
    }

    php_stream_to_zval(stream, zstream);

    return SUCCESS;
}

zend_always_inline static zend_bool string_contains_char(char *haystack, char chr)
{
    char *p = strchr(haystack, chr);

    return (p != NULL);
}

zend_always_inline static zend_bool stream_is_writable(php_stream *stream)
{
    return !string_contains_char(stream->mode, 'r') || string_contains_char(stream->mode, '+');
}

zend_always_inline static zend_bool stream_is_readable(php_stream *stream)
{
    return string_contains_char(stream->mode, 'r') || string_contains_char(stream->mode, '+');
}

#endif //HTTP_MESSAGE_STREAM_H
