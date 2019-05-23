--TEST--
Uri::getPath()
--FILE--
<?php
var_dump((new HttpMessage\Uri())->getPath());
var_dump((new HttpMessage\Uri('http://example.com'))->getPath());
var_dump((new HttpMessage\Uri('http://example.com/'))->getPath());
var_dump((new HttpMessage\Uri('http://example.com/foo'))->getPath());
var_dump((new HttpMessage\Uri('/'))->getPath());
var_dump((new HttpMessage\Uri('/foo'))->getPath());
?>
--EXPECT--
string(0) ""
string(0) ""
string(1) "/"
string(4) "/foo"
string(1) "/"
string(4) "/foo"
