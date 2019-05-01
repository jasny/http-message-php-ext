cmake_minimum_required(VERSION 3.8)
project(http_message)

add_custom_target(makefile COMMAND phpize && ./configure && make && make install
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES php_http_message
        http_message.c)

execute_process (
        COMMAND php-config --include-dir
        OUTPUT_VARIABLE PHP_SOURCE
)

include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PHP_SOURCE}/sapi)
include_directories(${PHP_SOURCE}/pear)
include_directories(${PHP_SOURCE}/TSRM)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(http_message ${SOURCE_FILES})
