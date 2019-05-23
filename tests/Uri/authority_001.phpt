--TEST--
Uri::getAuthority()
--FILE--
<?php
$uri = new HttpMessage\Uri('http://acme:secure@www.example.com:8000/foo?q=42#answer');
var_dump($uri->getAuthority());
?>
--EXPECT--
string(32) "acme:secure@www.example.com:8000"
