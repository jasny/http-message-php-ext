--TEST--
Uri::getAuthority()
--FILE--
<?php
$uri = new HttpMessage\Uri('https://www.example.com/');
var_dump($uri->getAuthority());
?>
--EXPECT--
string(15) "www.example.com"

