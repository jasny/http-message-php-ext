--TEST--
Cast Uri to string with query or fragment
--FILE--
<?php
var_dump((string)(new HttpMessage\Uri('?q=42')));
var_dump((string)(new HttpMessage\Uri('#answer')));
var_dump((string)(new HttpMessage\Uri('?q=42#answer')));
var_dump((string)(new HttpMessage\Uri('/foo?q=42#answer')));
?>
--EXPECT--
string(5) "?q=42"
string(7) "#answer"
string(12) "?q=42#answer"
string(16) "/foo?q=42#answer"
