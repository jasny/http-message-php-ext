--TEST--
Uri::getQuery()
--FILE--
<?php
var_dump((new HttpMessage\Uri())->getQuery());
var_dump((new HttpMessage\Uri('http://example.com?foo=1&bar=2'))->getQuery());
var_dump((new HttpMessage\Uri('?foo=1&bar=2'))->getQuery());
var_dump((new HttpMessage\Uri('https://www.example.com'))->getQuery());
?>
--EXPECT--
string(0) ""
string(11) "foo=1&bar=2"
string(11) "foo=1&bar=2"
string(0) ""
