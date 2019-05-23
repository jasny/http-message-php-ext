--TEST--
Cast Uri to string with paths
--FILE--
<?php
var_dump((string)(new HttpMessage\Uri('foo')));
var_dump((string)(new HttpMessage\Uri('/foo')));
?>
--EXPECT--
string(3) "foo"
string(4) "/foo"
