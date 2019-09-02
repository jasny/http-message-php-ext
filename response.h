/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension                                           |
  | Helper function to get reason phrase from status code                |
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

#ifndef HTTP_MESSAGE_RESPONSE_H
#define HTTP_MESSAGE_RESPONSE_H

#include "http_status_codes.h"

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

int response_set_status(zval *obj, zend_long code, zend_string *phrase);

#endif //HTTP_MESSAGE_RESPONSE_H
