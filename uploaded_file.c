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
#include "php_http_message.h"
#include "macros.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "SAPI.h"
#include "ext/standard/file.h"
#include "ext/psr/psr_http_message.h"
#include "ext/spl/spl_exceptions.h"

#if HAVE_HTTP_MESSAGE

zend_class_entry *HttpMessage_UploadedFile_ce = NULL;

int assert_file_available(zval *file, zval *stream, zval *moved)
{
    zval stream_file, arg1;
    char *filename;

    if ((file == NULL || ZVAL_IS_NULL(file)) && (stream == NULL || ZVAL_IS_NULL(stream))) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "No file was uploaded or uploaded file not available");
        return FAILURE;
    }

    if (Z_TYPE_P(moved) == IS_TRUE) {
        if (!ZVAL_IS_NULL(file)) {
            filename = Z_STRVAL_P(file);
            STR_CLOSE(filename, Z_STRLEN_P(file));
        } else {
            // Can be any StreamInterface object, doesn't need to be Stream from this lib.
            ZVAL_STRINGL(&arg1, "uri", 3);
            zend_call_method_with_1_params(PROPERTY_ARG(stream), NULL, NULL, "getMetadata", &stream_file, &arg1);
            filename = Z_STRVAL(stream_file);
            STR_CLOSE(filename, Z_STRLEN(stream_file));
        }

        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Uploaded file '%s' has already been moved", filename);
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

    STR_CLOSE(new_path, new_path_len);

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

int move_uploaded_stream(zval *stream, char *new_path, size_t new_path_len)
{
    zval resource;
    php_stream *source, *target;
    size_t len;
    int ret;

    // Can be any StreamInterface object, doesn't need to be Stream from this lib.
    zend_call_method_with_0_params(PROPERTY_ARG(stream), NULL, NULL, "detach", &resource);
    source = (php_stream*)zend_fetch_resource2_ex(&resource, "stream", php_file_le_stream(), php_file_le_pstream());

    STR_CLOSE(new_path, new_path_len);
    target = php_stream_open_wrapper(new_path, "w", 0, NULL);

    if (EXPECTED(source != NULL && target != NULL)) {
        if (source->ops->seek && (source->flags & PHP_STREAM_FLAG_NO_SEEK) == 0) {
            php_stream_seek(source, 0, SEEK_SET);
        };

        ret = php_stream_copy_to_stream_ex(source, target, PHP_STREAM_COPY_ALL, &len);
    } else {
        ret = FAILURE;
    }

    if (ret == FAILURE) {
        zend_throw_exception_ex(spl_ce_RuntimeException, 0, "Failed to stream uploaded file to '%s'", new_path);
    }

    return ret;
}

void construct_uploaded_file(
    zval* object,
    zval *stream,
    zend_string *file,
    zend_long size,
    zend_long error,
    zend_string *clientFilename,
    zend_string *clientMediaType,
    char checkUploaded
) {
    zval zreadable;

    if (error == 0 && stream != NULL) {
        // Must be verified it's a StreamInterface object before passing it to this function.
        // Can be any StreamInterface object, doesn't need to be Stream from this lib.
        zend_call_method_with_0_params(PROPERTY_ARG(stream), NULL, NULL, "isReadable", &zreadable);

        if (UNEXPECTED(Z_TYPE(zreadable) != IS_TRUE)) {
            zend_throw_exception(
                    spl_ce_InvalidArgumentException, "Stream provided for uploaded file is not readable", 0
            );
        }
        zend_update_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("stream"), stream);
    } else if (error == 0 && file != NULL) {
        zend_update_property_str(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("file"), file);
    }

    if (clientFilename != NULL) {
        zend_update_property_str(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("clientFilename"), clientFilename);
    }
    if (clientMediaType != NULL) {
        zend_update_property_str(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("clientMediaType"), clientMediaType);
    }

    if (size > 0) {
        zend_update_property_long(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("size"), size);
    }
    if (error < 0 || error > 8) {
        zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0, "Invalid error code %ld", error);
    }
    zend_update_property_long(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("error"), error);

    if (checkUploaded < 0) {
        checkUploaded = SG(rfc1867_uploaded_files) != NULL;
    }
    zend_update_property_bool(HttpMessage_UploadedFile_ce, PROPERTY_ARG(object), ZEND_STRL("checkUploaded"), checkUploaded);
}

void create_uploaded_file(zval *uploaded_file, zval *tmp_name, zval *size, zval *error, zval *name, zval *type)
{
    zend_object *new_object;

    new_object = zend_objects_new(HttpMessage_UploadedFile_ce);
    object_properties_init(new_object, HttpMessage_UploadedFile_ce);

    ZVAL_OBJ(uploaded_file, new_object);

    construct_uploaded_file(
            uploaded_file,
            NULL,
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
            restructure_uploaded_files(element, Z_ARR_P(name), Z_ARR_P(type), Z_ARR_P(tmp_name), Z_ARR_P(error),
                    Z_ARR_P(size));
        }
    } ZEND_HASH_FOREACH_END();
}


/* __construct */

ZEND_BEGIN_ARG_INFO_EX(arginfo_HttpMessageUploadedFile_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, fileOrStream)
    ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, error, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, clientFilename, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, clientMediaType, IS_STRING, 1)
    ZEND_ARG_INFO(0, checkUploaded)
ZEND_END_ARG_INFO()

PHP_METHOD(UploadedFile, __construct)
{
    zval *fileOrStream = NULL, *stream = NULL;
    zend_string *file = NULL, *clientFilename = NULL, *clientMediaType = NULL;
    zend_long size = -1, error = 0;
    zend_bool size_is_null = 1, checkUploaded = 0, checkUploaded_is_null = 1;
    zend_class_entry *stream_interface;

    ZEND_PARSE_PARAMETERS_START_EX(ZEND_PARSE_PARAMS_THROW, 1, 6)
        Z_PARAM_ZVAL(fileOrStream)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG_EX(size, size_is_null, 1, 0)
        Z_PARAM_LONG(error)
        Z_PARAM_STR_EX(clientFilename, 1, 0)
        Z_PARAM_STR_EX(clientMediaType, 1, 0)
        Z_PARAM_BOOL_EX(checkUploaded, checkUploaded_is_null, 1, 0)
    ZEND_PARSE_PARAMETERS_END();

    if (Z_TYPE_P(fileOrStream) == IS_STRING) {
        file = Z_STR_P(fileOrStream);
    } else if (Z_TYPE_P(fileOrStream) != IS_NULL) {
        stream_interface = HTTP_MESSAGE_PSR_INTERFACE("stream");

        if (UNEXPECTED(stream_interface == NULL)) {
            zend_throw_error(NULL, "Psr\\Http\\Message\\StreamInterface not found");
            return;
        }

        if (UNEXPECTED(
            Z_TYPE_P(fileOrStream) != IS_OBJECT ||
            !instanceof_function(Z_OBJCE_P(fileOrStream), stream_interface)
        )) {
            custom_parameter_type_error(1,
                    "a string or object that implements Psr\\Http\\Message\\StreamInterface", fileOrStream);
            return;
        }

        stream = fileOrStream;
    }

    construct_uploaded_file(getThis(), stream, file, size_is_null ? -1 : size, error, clientFilename, clientMediaType,
            checkUploaded_is_null ? -1 : checkUploaded);
}

PHP_METHOD(UploadedFile, getStream)
{
    zval rv, *stream, *file, *moved, mode;

    file = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("file"), 0, &rv);
    stream = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("stream"), 0, &rv);
    moved = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("moved"), 0, &rv);

    if (assert_file_available(file, stream, moved) == FAILURE) {
        return;
    }

    // Stream isn't set: create new stream for file
    if (ZVAL_IS_NULL(stream)) {
        ZVAL_STRINGL(&mode, "r", 1);
        NEW_OBJECT_CONSTRUCT(stream, HttpMessage_Stream_ce, 2, file, &mode);
    }

    RETURN_ZVAL(stream, 1, 0);
}

PHP_METHOD(UploadedFile, moveTo)
{
    zval rv, *file, *moved, *stream, *checkUploaded;
    char *new_path = NULL;
    size_t new_path_len = 0;
    int move_ret;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_PATH(new_path, new_path_len)
    ZEND_PARSE_PARAMETERS_END();

    file = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("file"), 0, &rv);
    stream = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("stream"), 0, &rv);
    moved = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("moved"), 0, &rv);
    checkUploaded = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("checkUploaded"), 0, &rv);

    if (
        assert_file_available(file, stream, moved) == FAILURE ||
        (Z_TYPE_P(checkUploaded) == IS_TRUE && assert_uploaded_file(Z_STRVAL_P(file), Z_STRLEN_P(file) == FAILURE))
    ) {
        return;
    }

    if (!ZVAL_IS_NULL(file)) {
        move_ret = move_uploaded_file(Z_STRVAL_P(file), Z_STRLEN_P(file), new_path, new_path_len);
    } else {
        move_ret = move_uploaded_stream(stream, new_path, new_path_len);
    }

    ZVAL_BOOL(moved, move_ret == SUCCESS);
}

PHP_METHOD(UploadedFile, getSize)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("size"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getError)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("error"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getClientFilename)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("clientFilename"), 0, &rv);

    RETURN_ZVAL(value, 1, 0);
}

PHP_METHOD(UploadedFile, getClientMediaType)
{
    zval rv, *value;

    value = zend_read_property(HttpMessage_UploadedFile_ce, PROPERTY_ARG(getThis()), ZEND_STRL("clientMediaType"), 0, &rv);

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
    zend_class_entry *interface = HTTP_MESSAGE_PSR_INTERFACE("uploadedfile");

    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND(interface, "UploadedFile");

    INIT_NS_CLASS_ENTRY(ce, "HttpMessage", "UploadedFile", methods);

    HttpMessage_UploadedFile_ce = zend_register_internal_class(&ce);
    zend_class_implements(HttpMessage_UploadedFile_ce, 1, interface);

    /* Properties */
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("stream"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("file"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("size"), ZEND_ACC_PRIVATE);
    zend_declare_property_long(HttpMessage_UploadedFile_ce, ZEND_STRL("error"), 0, ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("clientFilename"), ZEND_ACC_PRIVATE);
    zend_declare_property_null(HttpMessage_UploadedFile_ce, ZEND_STRL("clientMediaType"), ZEND_ACC_PRIVATE);

    zend_declare_property_bool(HttpMessage_UploadedFile_ce, ZEND_STRL("moved"), 0, ZEND_ACC_PRIVATE);
    zend_declare_property_bool(HttpMessage_UploadedFile_ce, ZEND_STRL("checkUploaded"), 0, ZEND_ACC_PRIVATE);

    return SUCCESS;
}

#endif
