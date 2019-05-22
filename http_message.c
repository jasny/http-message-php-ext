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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_http_message.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"

#if HAVE_HTTP_MESSAGE

static const zend_module_dep deps[] = {
    ZEND_MOD_REQUIRED("psr")
};

zend_module_entry http_message_module_entry = {
    STANDARD_MODULE_HEADER_EX,
    NULL,
    deps,
    PHP_HTTP_MESSAGE_EXTNAME,
    NULL,
    PHP_MINIT(http_message),
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_HTTP_MESSAGE_VERSION,
    STANDARD_MODULE_PROPERTIES
};

PHP_MINIT_FUNCTION(http_message)
{
    PHP_MINIT(http_message_message)(INIT_FUNC_ARGS_PASSTHRU);
    PHP_MINIT(http_message_request)(INIT_FUNC_ARGS_PASSTHRU);
    PHP_MINIT(http_message_serverrequest)(INIT_FUNC_ARGS_PASSTHRU);
    PHP_MINIT(http_message_response)(INIT_FUNC_ARGS_PASSTHRU);
    PHP_MINIT(http_message_stream)(INIT_FUNC_ARGS_PASSTHRU);
    PHP_MINIT(http_message_uri)(INIT_FUNC_ARGS_PASSTHRU);

    return SUCCESS;
}

#ifdef COMPILE_DL_HTTP_MESSAGE
ZEND_GET_MODULE(http_message)
#endif

#endif
