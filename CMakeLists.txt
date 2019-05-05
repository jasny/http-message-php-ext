cmake_minimum_required(VERSION 3.8)
project(http_message)

add_custom_target(extension COMMAND phpize && ./configure && make && make install
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES php_http_message
        http_message.c message.c request.c uri.c server_request.c response.c)

execute_process (
        COMMAND php-config --include-dir
        OUTPUT_VARIABLE PHP_SOURCE
)
string(REGEX REPLACE "\n$" "" PHP_SOURCE "${PHP_SOURCE}")

message("Using source directory: ${PHP_SOURCE}")

include_directories(${PHP_SOURCE})
include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(http_message ${SOURCE_FILES})
