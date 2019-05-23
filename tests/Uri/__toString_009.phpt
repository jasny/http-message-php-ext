--TEST--
Cast Uri to string with authority but without schema
--FILE--
<?php
$uri = new HttpMessage\Uri('http://www.example.com');
var_dump((string)($uri->withPath('foo')));
var_dump((string)($uri->withPath('//foo')));
?>
--EXPECT--
string(26) "http://www.example.com/foo"
string(27) "http://www.example.com//foo"
