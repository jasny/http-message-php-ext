# PSR-7 HTTP Message as Pecl extension

[![Build Status](https://travis-ci.org/jasny/http_message-php-ext.svg?branch=master)](https://travis-ci.org/jasny/http_message-php-ext)
[![Build status](https://ci.appveyor.com/api/projects/status/7rof1vr8mv4kam17/branch/master?svg=true)](https://ci.appveyor.com/project/jasny/http_message-php-ext/branch/master)

[PSR-7 HTTP Message](https://www.php-fig.org/psr/psr-7/) implementation as PHP extension written in C.

---

## Requirements

* PHP 7.x

## Installation

    phpize
    ./configure
    make
    make test
    make install

Add the following line to your `php.ini`

    extension=http_message.so

To try out the extension, you can run the following command

    php -a -d extension=modules/http_message.so

