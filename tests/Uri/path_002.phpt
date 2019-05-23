--TEST--
Uri::withPath()
--FILE--
<?php
$blank = new HttpMessage\Uri();
$root = $blank->withPath("/");
$absolute = $root->withPath("/foo");
$relative = $absolute->withPath("foo");
$double = $absolute->withPath("//foo");

var_dump($blank->getPath());
var_dump($root->getPath());
var_dump($absolute->getPath());
var_dump($relative->getPath());
var_dump($double->getPath());
?>
--EXPECT--
string(0) ""
string(1) "/"
string(4) "/foo"
string(3) "foo"
string(5) "//foo"
