--TEST--
Uri::getHost()
--FILE--
<?php
var_dump((new HttpMessage\Uri())->getHost());
var_dump((new HttpMessage\Uri('http://www.example.com'))->getHost());
?>
--EXPECT--
string(0) ""
string(15) "www.example.com"
