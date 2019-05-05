dnl $Id$
dnl config.m4 for extension http_message

PHP_ARG_WITH(http_message, for http_message support,
[  --with-http_message       Include http_message support])

if test "$PHP_HTTP_MESSAGE" != "no"; then
  # TODO: Load external depenencies here  

  AC_DEFINE(HAVE_HTTP_MESSAGE, 1, [Whether you have http_message support])
  PHP_NEW_EXTENSION(http_message, http_message.c message.c request.c server_request.c response.c uri.c, $ext_shared)
fi

