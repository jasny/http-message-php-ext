![improved PHP library](https://user-images.githubusercontent.com/100821/46372249-e5eb7500-c68a-11e8-801a-2ee57da3e5e3.png)

# PSR-7 HTTP Message as PHP extension

[![Build Status](https://travis-ci.org/improved-php-library/http-message.svg?branch=master)](https://travis-ci.org/improved-php-library/http-message)
[![Build status](https://ci.appveyor.com/api/projects/status/7rof1vr8mv4kam17/branch/master?svg=true)](https://ci.appveyor.com/project/jasny/http-message/branch/master)

[PSR-7 HTTP Message](https://www.php-fig.org/psr/psr-7/) implementation as PHP extension written in C.

---

## Requirements

* PHP 7.2+
* [psr extension 0.6+](https://github.com/jbboehr/php-psr)

## Installation

The extension is [available from pecl](https://pecl.php.net/package/http_message).

    pecl install psr-beta
    pecl install http_message-beta

### Manual build

Instead of installing this extension from pecl, you can build it manually

    phpize
    ./configure
    make
    make test
    make install

Add the following line to your `php.ini`

    extension=http_message.so

To try out the extension, you can run the following command

    php -a -d extension=modules/http_message.so
