--TEST--
Uri::getFragment()
--FILE--
<?php
var_dump((new HttpMessage\Uri())->getFragment());
var_dump((new HttpMessage\Uri('http://example.com#foo'))->getFragment());
var_dump((new HttpMessage\Uri('#foo'))->getFragment());
var_dump((new HttpMessage\Uri('?a=1#foo'))->getFragment());
?>
--EXPECT--
string(0) ""
string(3) "foo"
string(3) "foo"
string(3) "foo"
