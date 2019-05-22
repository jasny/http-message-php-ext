dnl $Id$
dnl config.m4 for extension http_message

sinclude(./autoconf/pecl.m4)

PECL_INIT([http_message])

PHP_ARG_WITH(http-message, for http_message support,
[  --with-http-message       Include http_message support])

if test "$PHP_HTTP_MESSAGE" != "no"; then
	PECL_HAVE_PHP_EXT([psr], [
		PECL_HAVE_PHP_EXT_HEADER([psr])
	], [
		AC_MSG_ERROR([please install and enable pecl/psr])
	])

    AC_DEFINE(HAVE_HTTP_MESSAGE, 1, [Whether you have http_message support])
    PHP_NEW_EXTENSION(http_message, http_message.c message.c request.c server_request.c response.c stream.c uri.c,
          $ext_shared)

	PHP_ADD_EXTENSION_DEP([http_message], [psr], true)
fi
