/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension - UploadedFile class                      |
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
#include "macros.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"
#include "ext/standard/info.h"
#include "ext/standard/file.h"
#include "ext/psr/psr_http_message.h"
#include "ext/spl/spl_exceptions.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_UploadedFile_ce;

void uploaded_file_chmod(char *new_path)
{
    int oldmask; int ret;

    oldmask = umask(077);
    umask(oldmask);

    ret = VCWD_CHMOD(new_path, 0666 & ~oldmask);

    if (ret == -1) {
        php_error_docref(NULL, E_WARNING, "%s", strerror(errno));
    }
}

zend_bool move_uploaded_file(char *path, size_t path_len, char *new_path, size_t new_path_len)
{
    zend_bool successful = 0;

    if (SG(rfc1867_uploaded_files) && !zend_hash_str_exists(SG(rfc1867_uploaded_files), path, path_len)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Unable to move '%s'; not an uploaded file", path);
        return 0;
    }

    if (php_check_open_basedir_ex(new_path, 1)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0,
                "Unable to move '%s' to '%s'; open_basedir restriction in effect", path, new_path);
        return 0;
    }

    if (VCWD_RENAME(path, new_path) == 0) {
        successful = 1;
#ifndef PHP_WIN32
        uploaded_file_chmod(new_path);
#endif
    } else if (php_copy_file_ex(path, new_path, STREAM_DISABLE_OPEN_BASEDIR) == SUCCESS) {
        VCWD_UNLINK(path);
        successful = 1;
    }

    if (!successful) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Unable to move '%s' to '%s'", path, new_path);
        return 0;
    }

    zend_hash_str_del(SG(rfc1867_uploaded_files), path, path_len);
    return 1;
}

/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUploadedFile_construct, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, uri, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(UploadedFile, __construct)
{
    zend_string *file, *clientFilename = NULL, *clientMediaType = NULL;
    zend_long size = 0, error = 0;
    zend_bool size_is_null;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 0, 1)
        Z_PARAM_STR(file)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG_EX(size, size_is_null, 1, 0)
        Z_PARAM_LONG(error)
        Z_PARAM_STR_EX(clientFilename, 1, 0)
        Z_PARAM_STR(clientMediaType)
    ZEND_PARSE_PARAMETERS_END();

    zend_update_property_str(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("file"), file);

    if (!size_is_null) {
        zend_update_property_long(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("size"), size);
    }
    zend_update_property_long(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("error"), error);

    SET_STR_PROPERTY(HttpMessage_UploadedFile_ce, "clientFilename", clientFilename);
    SET_STR_PROPERTY(HttpMessage_UploadedFile_ce, "clientMediaType", clientMediaType);
}

PHP_METHOD(UploadedFile, getStream)
{
    zval rv, *stream, *file, *zmoved, resource;
    php_stream *resource_stream;
    zend_bool opened;

    stream = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("stream"), 0, &rv);

    file = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("file"), 0, &rv);
    zmoved = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("moved"), 0, &rv);

    if (Z_TYPE_P(zmoved) == IS_TRUE) {
        zend_throw_exception(spl_ce_RuntimeException, "File '%s' has already been moved", Z_STRVAL_P(file));
        return;
    }

    // Stream isn't set: create new stream for file
    if (ZVAL_IS_NULL(stream)) {
        resource_stream = php_stream_fopen(Z_STRVAL_P(file), 'r', &opened);
        if (!opened) {
            zend_throw_exception(spl_ce_RuntimeException, "Failed to open stream for '%s'", Z_STRVAL_P(file));
            return;
        }

        php_stream_to_zval(resource_stream, &resource);

        object_init_ex(stream, HttpMessage_Stream_ce);
        if (stream != NULL) { /* Should always be true */
            zend_call_method_with_1_params(
                    stream, HttpMessage_Stream_ce, &HttpMessage_Stream_ce->constructor, "__construct", NULL, &resource
            );
        }
    }

    RETURN_ZVAL(stream, 1, 0);
}

PHP_METHOD(UploadedFile, moveTo)
{
    zval rv, *file, *zmoved;
    char *new_path;
    size_t new_path_len;
    zend_bool moved;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_PATH(new_path, new_path_len)
    ZEND_PARSE_PARAMETERS_END();

    file = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("file"), 0, &rv);
    zmoved = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("moved"), 0, &rv);

    if (Z_TYPE_P(zmoved) == IS_TRUE) {
        zend_throw_exception(spl_ce_RuntimeException, "File '%s' has already been moved", Z_STRVAL_P(file));
        return;
    }

    moved = move_uploaded_file(Z_STRVAL_P(file), Z_STRLEN_P(file), new_path, new_path_len);

    zend_update_property_bool(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("moved"), moved);
}

PHP_METHOD(UploadedFile, getSize)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("body"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getError)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("body"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getClientFilename)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("body"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getClientMediaType)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_Message_ce, getThis(), ZEND_STRL("body"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

/* Define HttpMessage\UploadedFile class */

static const zend_function_entry uri_functions[] = {
    PHP_ME(UploadedFile, __construct, arginfo_HttpMessageUploadedFile_construct, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(UploadedFile, getStream)
    HTTP_MESSAGE_ME(UploadedFile, moveTo)
    HTTP_MESSAGE_ME(UploadedFile, getSize)
    HTTP_MESSAGE_ME(UploadedFile, getError)
    HTTP_MESSAGE_ME(UploadedFile, getClientFilename)
    HTTP_MESSAGE_ME(UploadedFile, getClientMediaType)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_uri)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "UploadedFile", uri_functions);

    HttpMessage_UploadedFile_ce = zend_register_internal_class(&ce);
    zend_class_implements(HttpMessage_UploadedFile_ce, 1, PsrHttpMessageUploadedFileInterface_ce_ptr);

    /* Properties */
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("stream"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("file"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("size"), ZEND_ACC_PROTECTED);
    zend_declare_property_long(HttpMessage_UploadedFile_ce, ZEND_STRL("error"), 0, ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("clientFilename"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("clientMediaType"), ZEND_ACC_PROTECTED);

    zend_declare_property_bool(HttpMessage_UploadedFile_ce, ZEND_STRL("moved"), ZEND_ACC_PRIVATE, 0);

    return SUCCESS;
}

#endif
