--TEST--
Cast Uri to string with full url
--FILE--
<?php
$uri = new HttpMessage\Uri('http://acme:secure@www.example.com:8000/foo?q=42#answer');
var_dump((string)$uri);
?>
--EXPECT--
string(55) "http://acme:secure@www.example.com:8000/foo?q=42#answer"
