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

int assert_file_available(zval *file, zval *moved)
{
    if (ZVAL_IS_NULL(file)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "No file was uploaded or uploaded file not available");
        return FAILURE;
    }

    if (Z_TYPE_P(moved) == IS_TRUE) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Uploaded file '%s' has already been moved",
                Z_STRVAL_P(file));
        return FAILURE;
    }

    return SUCCESS;
}

zend_bool assert_uploaded_file(char *path, size_t path_len)
{
    if (SG(rfc1867_uploaded_files) == NULL || !zend_hash_str_exists(SG(rfc1867_uploaded_files), path, path_len)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Won't move '%s'; not an uploaded file", path);
        return FAILURE;
    }

    return SUCCESS;
}

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

int move_uploaded_file(char *path, size_t path_len, char *new_path, size_t new_path_len)
{
    HashTable *uploaded_files;
    zend_bool successful = 0;

    if (php_check_open_basedir_ex(new_path, 1)) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0,
                 "Unable to move uploaded file '%s' to '%s'; open_basedir restriction in effect", path, new_path);
        return FAILURE;
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
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to move uploaded file '%s' to '%s'",
                path, new_path);
        return FAILURE;
    }

    uploaded_files = SG(rfc1867_uploaded_files);
    if (uploaded_files != NULL) {
        zend_hash_str_del(uploaded_files, path, path_len);
    }

    return SUCCESS;
}

void construct_uploaded_file(
    zval* object,
    zend_string *file,
    zend_long size,
    zend_long error,
    zend_string *clientFilename,
    zend_string *clientMediaType,
    char checkUploaded
) {
    if (error == 0 && file != NULL) {
        zend_update_property_str(HttpMessage_UploadedFile_ce, object, ZEND_STRL("file"), file);
    }
    if (clientFilename != NULL) {
        zend_update_property_str(HttpMessage_UploadedFile_ce, object, ZEND_STRL("clientFilename"), clientFilename);
    }
    if (clientMediaType != NULL) {
        zend_update_property_str(HttpMessage_UploadedFile_ce, object, ZEND_STRL("clientMediaType"), clientMediaType);
    }

    if (size > 0) {
        zend_update_property_long(HttpMessage_UploadedFile_ce, object, ZEND_STRL("size"), size);
    }
    if (error < 0 || error > 8) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Invalid error code %ld", error);
    }
    zend_update_property_long(HttpMessage_UploadedFile_ce, object, ZEND_STRL("error"), error);

    if (checkUploaded < 0) {
        checkUploaded = SG(rfc1867_uploaded_files) != NULL;
    }
    zend_update_property_bool(HttpMessage_UploadedFile_ce, object, ZEND_STRL("checkUploaded"), checkUploaded);
}

void create_uploaded_file(zval *uploaded_file, zval *tmp_name, zval *size, zval *error, zval *name, zval *type)
{
    ZVAL_OBJ(uploaded_file, zend_objects_new(HttpMessage_UploadedFile_ce));
    return;

    construct_uploaded_file(
            uploaded_file,
            tmp_name != NULL ? Z_STR_P(tmp_name) : NULL,
            size != NULL ? Z_LVAL_P(size) : -1,
            Z_LVAL_P(error),
            name != NULL ? Z_STR_P(name) : NULL,
            type != NULL ? Z_STR_P(type) : NULL,
            -1 // auto-detect check is_uploaded
    );
}

void restructure_uploaded_files(
    zval *objects,
    HashTable *names,
    HashTable *types,
    HashTable *tmp_names,
    HashTable *errors,
    HashTable *sizes
) {
    zval *name, *type, *tmp_name, *error, *size, *element;
    zend_ulong index;
    zend_string *key;

    ZEND_HASH_FOREACH_KEY_VAL(errors, index, key, error) {
        element = ARRAY_ADD(Z_ARR_P(objects), index, key);

        name = ARRAY_GET(names, index, key);
        type = ARRAY_GET(types, index, key);
        tmp_name = ARRAY_GET(tmp_names, index, key);
        size = ARRAY_GET(sizes, index, key);

        if (Z_TYPE_P(error) == IS_LONG) {
            create_uploaded_file(element, tmp_name, size, error, name, type);
        } else if (EXPECTED(Z_TYPE_P(error) == IS_ARRAY)) {
            array_init(element);
            restructure_uploaded_files(element, Z_ARR_P_NULL(name), Z_ARR_P_NULL(type), Z_ARR_P_NULL(tmp_name),
                                       Z_ARR_P_NULL(error), Z_ARR_P_NULL(size)); // recursion
        }
    } ZEND_HASH_FOREACH_END();
}

void create_uploaded_files(zval *objects, HashTable *files)
{
    zval *zentry, *name, *type, *tmp_name, *error, *size, *element;
    HashTable *entry;
    zend_ulong index;
    zend_string *key;

    array_init(objects);

    ZEND_HASH_FOREACH_KEY_VAL(files, index, key, zentry) {
        if (UNEXPECTED(Z_TYPE_P(zentry) != IS_ARRAY)) continue;

        entry = Z_ARR_P(zentry);
        error = zend_hash_str_find(entry, ZEND_STRL("error"));

        if (UNEXPECTED(error == NULL)) continue;

        name = zend_hash_str_find(entry, ZEND_STRL("name"));
        type = zend_hash_str_find(entry, ZEND_STRL("type"));
        tmp_name = zend_hash_str_find(entry, ZEND_STRL("tmp_name"));
        size = zend_hash_str_find(entry, ZEND_STRL("size"));

        element = ARRAY_ADD(Z_ARR_P(objects), index, key);

        if (Z_TYPE_P(error) == IS_LONG) {
            create_uploaded_file(element, tmp_name, size, error, name, type);
        } else if (EXPECTED(Z_TYPE_P(error) == IS_ARRAY)) {
            array_init(element);
            restructure_uploaded_files(element, Z_ARR_P(name), Z_ARR_P(type), Z_ARR_P(tmp_name), Z_ARR_P(error)
                    , Z_ARR_P(size));
        }
    } ZEND_HASH_FOREACH_END();
}


/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUploadedFile_construct, 0, 1, 0)
    ZEND_ARG_TYPE_INFO(0, file, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, error, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, clientFilename, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, clientMediaType, IS_STRING, 1)
    ZEND_ARG_INFO(0, checkUploaded)
ZEND_END_ARG_INFO()

PHP_METHOD(UploadedFile, __construct)
{
    zend_string *file = NULL, *clientFilename = NULL, *clientMediaType = NULL;
    zend_long size = -1, error = 0;
    zend_bool size_is_null = 1, checkUploaded = 0, checkUploaded_is_null = 1;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 6)
        Z_PARAM_STR_EX(file, 1, 0)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG_EX(size, size_is_null, 1, 0)
        Z_PARAM_LONG(error)
        Z_PARAM_STR_EX(clientFilename, 1, 0)
        Z_PARAM_STR_EX(clientMediaType, 1, 0)
        Z_PARAM_BOOL_EX(checkUploaded, checkUploaded_is_null, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    construct_uploaded_file(getThis(), file, size_is_null ? -1 : size, error, clientFilename, clientMediaType,
            checkUploaded_is_null ? -1 : checkUploaded);
}

PHP_METHOD(UploadedFile, getStream)
{
    zval rv, *stream, *file, *moved, resource;
    php_stream *resource_stream;

    stream = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("stream"), 0, &rv);

    file = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("file"), 0, &rv);
    moved = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("moved"), 0, &rv);

    if (assert_file_available(file, moved) == FAILURE) {
        return;
    }

    // Stream isn't set: create new stream for file
    if (ZVAL_IS_NULL(stream)) {
        resource_stream = php_stream_open_wrapper(Z_STRVAL_P(file), "r", 0, NULL);
        if (resource_stream == NULL) {
            zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to open stream for '%s'", Z_STRVAL_P(file));
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
    zval rv, *file, *moved, *checkUploaded;
    char *new_path;
    size_t new_path_len;
    int move_ret;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_PATH(new_path, new_path_len)
    ZEND_PARSE_PARAMETERS_END();

    file = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("file"), 0, &rv);
    moved = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("moved"), 0, &rv);
    checkUploaded = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("checkUploaded"), 0, &rv);

    if (
        assert_file_available(file, moved) == FAILURE ||
        (Z_TYPE_P(checkUploaded) == IS_TRUE && assert_uploaded_file(Z_STRVAL_P(file), Z_STRLEN_P(file)) == FAILURE)
    ) {
        return;
    }

    move_ret = move_uploaded_file(Z_STRVAL_P(file), Z_STRLEN_P(file), new_path, new_path_len);
    ZVAL_BOOL(moved, move_ret == SUCCESS);
}

PHP_METHOD(UploadedFile, getSize)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("size"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getError)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("error"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getClientFilename)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("clientFilename"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getClientMediaType)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, getThis(), ZEND_STRL("clientMediaType"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

/* Define HttpMessage\UploadedFile class */

static const zend_function_entry methods[] = {
    PHP_ME(UploadedFile, __construct, arginfo_HttpMessageUploadedFile_construct, ZEND_ACC_PUBLIC)
    HTTP_MESSAGE_ME(UploadedFile, getStream)
    HTTP_MESSAGE_ME(UploadedFile, moveTo)
    HTTP_MESSAGE_ME(UploadedFile, getSize)
    HTTP_MESSAGE_ME(UploadedFile, getError)
    HTTP_MESSAGE_ME(UploadedFile, getClientFilename)
    HTTP_MESSAGE_ME(UploadedFile, getClientMediaType)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(http_message_uploadedfile)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "UploadedFile", methods);

    HttpMessage_UploadedFile_ce = zend_register_internal_class(&ce);
    zend_class_implements(HttpMessage_UploadedFile_ce, 1, PsrHttpMessageUploadedFileInterface_ce_ptr);

    /* Properties */
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("stream"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("file"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("size"), ZEND_ACC_PROTECTED);
    zend_declare_property_long(HttpMessage_UploadedFile_ce, ZEND_STRL("error"), 0, ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("clientFilename"), ZEND_ACC_PROTECTED);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("clientMediaType"), ZEND_ACC_PROTECTED);

    zend_declare_property_bool(HttpMessage_UploadedFile_ce, ZEND_STRL("moved"), 0, ZEND_ACC_PROTECTED);
    zend_declare_property_bool(HttpMessage_UploadedFile_ce, ZEND_STRL("checkUploaded"), 0, ZEND_ACC_PROTECTED);

    return SUCCESS;
}

#endif
