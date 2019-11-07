cmake_minimum_required(VERSION 3.8)
project(http_message C)

add_compile_definitions(HAVE_HTTP_MESSAGE)

set(SOURCE_FILES php_http_message
        http_message.c
        message.c
        request.c
        server_request.c
        response.c
        uri.c
        stream.c
        uploaded_file.c
        factory.c
        emitter.c
)

execute_process (
        COMMAND php-config --include-dir
        OUTPUT_VARIABLE PHP_SOURCE
)
string(REGEX REPLACE "\n$" "" PHP_SOURCE "${PHP_SOURCE}")

message("Using source directory: ${PHP_SOURCE}")

include_directories(${PHP_SOURCE})
include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PHP_SOURCE}/TSRM)
include_directories(${PROJECT_SOURCE_DIR})

add_custom_target(configure
        COMMAND phpize && ./configure
        DEPENDS ${SOURCE_FILES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_library(___ EXCLUDE_FROM_ALL ${SOURCE_FILES})
