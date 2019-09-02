/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension                                           |
  | Generic macros for this library                                      |
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

#define SET_ARRAY_PROPERTY(className, property, zval, rv) \
        if (zval == NULL) array_init(zend_read_property(className, getThis(), ZEND_STRL(property), 0, &rv)); \
        else zend_update_property(className, getThis(), ZEND_STRL(property), zval)

#define SET_STRING_PROPERTY(className, property, val) \
        if (val != NULL) zend_update_property_stringl(className, getThis(), ZEND_STRL(property), val, strlen(val))

#define SET_STR_PROPERTY(className, property, val) \
        if (val != NULL) zend_update_property_str(className, getThis(), ZEND_STRL(property), val)

#if PHP_VERSION_ID < 70300
#define SET_URI_PROPERTY(className, property, val) SET_STRING_PROPERTY(className, property, val)
#else
#define SET_URI_PROPERTY(className, property, val) SET_STR_PROPERTY(className, property, val)
#endif

#define HTTP_MESSAGE_ME_EX(className, interfaceName, method) \
        PHP_ME(className, method, arginfo_PsrHttpMessage ## interfaceName ## Interface_ ## method, ZEND_ACC_PUBLIC)

#define HTTP_MESSAGE_ME(className, method) \
        HTTP_MESSAGE_ME_EX(className, className, method)

#define HTTP_MESSAGE_PSR_INTERFACE(interfaceName) \
        get_internal_ce(ZEND_STRL("psr\\http\\message\\" interfaceName "interface"))

#define NEW_OBJECT(zval, ce) \
        object_init_ex(zval, ce); \
        if (EXPECTED(zval != NULL)) object_properties_init(Z_OBJ_P(zval), ce)

#define NEW_OBJECT_CONSTRUCT(zval, ce, argc, ...) \
        NEW_OBJECT(zval, ce); \
        if (EXPECTED(zval != NULL)) \
            zend_call_method_with_## argc ##_params( \
                    zval, ce, &ce->constructor, "__construct", NULL, ##__VA_ARGS__ \
            )

#define IS_STREAM_RESOURCE(zstream) \
        ( \
            Z_TYPE_P(zstream) == IS_RESOURCE && \
            (Z_RES_P(zstream)->type == php_file_le_stream() || Z_RES_P(zstream)->type == php_file_le_pstream()) \
        )

#define ARRAY_ADD(arr, index, key) \
        key == NULL ? zend_hash_index_add_empty_element(arr, index) : zend_hash_add_empty_element(arr, key)

#define ARRAY_GET(arr, index, key) \
        arr != NULL ? (key == NULL ? zend_hash_index_find(arr, index) : zend_hash_find(arr, key)) : NULL

#define COPY_PROPERTY_FROM_ARRAY(arr, key, className, object, property, type, val) \
        val = zend_hash_str_find(arr, ZEND_STRL(key)); \
        if (val != NULL && EXPECTED(Z_TYPE_P(val) == type)) { \
            zend_update_property(object, className, ZEND_STRL(property), val); \
        }

#define Z_ARR_P_NULL(zval) zval != NULL ? Z_ARR_P(zval) : NULL

#define Z_STRCMP(zval, match, def) \
        (zval == NULL || Z_TYPE_P(zval) != IS_STRING || Z_STRLEN_P(zval) == 0 ? def : \
        strncmp(match, Z_STRVAL_P(zval), Z_STRLEN_P(zval)))

#define Z_STRVAL_P_NULL(zval) zval != NULL ? Z_STRVAL_P(zval) : NULL
#define Z_STRLEN_P_NULL(zval) zval != NULL ? Z_STRLEN_P(zval) : 0

#define ZSTR_VAL_LEN(zstr) zstr != NULL ? ZSTR_VAL(zstr) : NULL, zstr != NULL ? ZSTR_LEN(zstr) : 0

#define STRLEN_NULL(str) str != NULL ? strlen(str) : 0

#define STR_CLOSE(str, len) str[len] = '\0'  // Is buffer big enough? yolo!

static zend_always_inline zend_class_entry* get_internal_ce(const char *class_name, size_t class_name_len)
{
    zend_class_entry* temp_ce;

    if ((temp_ce = zend_hash_str_find_ptr(CG(class_table), class_name, class_name_len)) == NULL) {
        return NULL;
    }

    return temp_ce;
}

#define ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(ce, className, psrClassName) \
    if (UNEXPECTED(ce == NULL)) { \
        zend_error(E_CORE_WARNING, \
                "Failed to initialize 'HttpMessage\\%s': 'Psr\\Http\\Message\\%sInterace' not found", \
                className, psrClassName); \
        return FAILURE; \
    }

#define ASSERT_HTTP_MESSAGE_INTERFACE_FOUND(ce, className) \
    ASSERT_HTTP_MESSAGE_INTERFACE_FOUND_EX(ce, className, className)

#endif //HTTP_MESSAGE_MACROS_H
