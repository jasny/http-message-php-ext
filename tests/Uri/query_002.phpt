--TEST--
Uri::withQuery()
--FILE--
<?php
$blank = new HttpMessage\Uri();
$foo = $blank->withQuery("foo=1");
$fooBar = $foo->withQuery("foo=1&bar=2");

var_dump($blank->getQuery());
var_dump($foo->getQuery());
var_dump($fooBar->getQuery());
?>
--EXPECT--
string(0) ""
string(5) "foo=1"
string(11) "foo=1&bar=2"
