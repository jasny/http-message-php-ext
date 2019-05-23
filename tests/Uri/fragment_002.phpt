--TEST--
Uri::withFragment()
--FILE--
<?php
$blank = new HttpMessage\Uri();
$foo = $blank->withFragment("foo");
$bar = $foo->withFragment("!bar");

var_dump($blank->getFragment());
var_dump($foo->getFragment());
var_dump($bar->getFragment());
?>
--EXPECT--
string(0) ""
string(3) "foo"
string(4) "!bar"
