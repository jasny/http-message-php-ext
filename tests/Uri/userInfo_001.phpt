--TEST--
Uri::getUserInfo()
--FILE--
<?php
var_dump((new HttpMessage\Uri)->getUserInfo());
var_dump((new HttpMessage\Uri('http://user@www.example.com'))->getUserInfo());
var_dump((new HttpMessage\Uri('http://user:pass@www.example.com'))->getUserInfo());
?>
--EXPECT--
string(0) ""
string(4) "user"
string(9) "user:pass"
