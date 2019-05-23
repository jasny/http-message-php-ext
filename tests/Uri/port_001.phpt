--TEST--
Uri::getPort()
--FILE--
<?php
var_dump((new HttpMessage\Uri)->getPort());
var_dump((new HttpMessage\Uri('http://www.example.com'))->getPort());
var_dump((new HttpMessage\Uri('http://www.example.com:80'))->getPort());
?>
--EXPECT--
NULL
NULL
int(80)
