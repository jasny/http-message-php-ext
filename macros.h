/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension                                           |
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


#ifndef HTTP_MESSAGE_MACROS_H
#define HTTP_MESSAGE_MACROS_H

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, 0, 0)
ZEND_END_ARG_INFO()

#define INIT_ARRAY_PROPERTY(className, property, rv) \
        array_init(zend_read_property(className, getThis(), ZEND_STRL(property), 0, &rv))

#define SET_STRING_PROPERTY(className, property, val) \
        if (val != NULL) \
            zend_update_property_stringl(className, getThis(), ZEND_STRL(property), val, strlen(val))

#define SET_STR_PROPERTY(className, property, val) \
        if (val != NULL) \
            zend_update_property_str(className, getThis(), ZEND_STRL(property), val)

#define HTTP_MESSAGE_ME(className, method) \
        PHP_ME(className, method, arginfo_PsrHttpMessage ## className ## Interface_ ## method, ZEND_ACC_PUBLIC)

#define IS_STREAM_RESOURCE(zstream) \
        ( \
            Z_TYPE_P(zstream) == IS_RESOURCE && \
            (Z_RES_P(zstream)->type == php_file_le_stream() || Z_RES_P(zstream)->type == php_file_le_pstream()) \
        )

#endif //HTTP_MESSAGE_MACROS_H
