--TEST--
Cast Uri to string with authority but without schema
--FILE--
<?php
var_dump((string)((new HttpMessage\Uri)->withPath('/foo')));
var_dump((string)((new HttpMessage\Uri)->withPath('//foo')));
var_dump((string)((new HttpMessage\Uri)->withPath('/////foo')));
?>
--EXPECT--
string(4) "/foo"
string(4) "/foo"
string(4) "/foo"
