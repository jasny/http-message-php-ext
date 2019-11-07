dnl $Id$
dnl config.m4 for extension http_message

sinclude(./autoconf/pecl.m4)
sinclude(./autoconf/php-executable.m4)

PECL_INIT([http_message])

PHP_ARG_ENABLE(http-message, whether to enable http-message, [ --enable-http-message   Enable http_message])

if test "$PHP_HTTP_MESSAGE" != "no"; then
	PECL_HAVE_PHP_EXT([psr], [PECL_HAVE_PHP_EXT_HEADER([psr])], [AC_MSG_ERROR([please install and enable the psr extension])])

    AC_DEFINE(HAVE_HTTP_MESSAGE, 1, [Whether you have http_message])
    PHP_NEW_EXTENSION(http_message, http_message.c message.c request.c server_request.c response.c stream.c uri.c uploaded_file.c factory.c emitter.c, $ext_shared)

	PHP_ADD_MAKEFILE_FRAGMENT
	PHP_INSTALL_HEADERS([ext/http_message], [php_http_message.h])
fi
