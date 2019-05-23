--TEST--
Uri::getScheme()
--FILE--
<?php
var_dump((new HttpMessage\Uri())->getScheme());
var_dump((new HttpMessage\Uri('http:'))->getScheme());
var_dump((new HttpMessage\Uri('mailto:john@example.com'))->getScheme());
var_dump((new HttpMessage\Uri('https://www.example.com'))->getScheme());
var_dump((new HttpMessage\Uri('ftp://www.example.com'))->getScheme());
?>
--EXPECT--
string(0) ""
string(4) "http"
string(6) "mailto"
string(5) "https"
string(3) "ftp"
