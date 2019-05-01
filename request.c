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
#include "zend_exceptions.h"
#include "ext/standard/info.h"

/* TODO: Add any includes to external libraries */

#if HAVE_HTTP_MESSAGE

/* TODO: Add all functions. (Keep PHP_FE_END as last element) */
zend_class_entry *request_ce;

static const zend_function_entry request_functions[] = {
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_request)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "HttpMessage\Request", request_functions);

    test_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);

    return SUCCESS;
}

/* TODO: Replace the example function for something better */

#endif

