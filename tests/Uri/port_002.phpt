--TEST--
Uri::withPort()
--FILE--
<?php
$blank = new HttpMessage\Uri('http://example.com');
$port80 = $blank->withPort(80);
$port8000 = $port80->withPort(8000);
$noPort = $port80->withPort(null);

var_dump($blank->getPort());
var_dump($port80->getPort());
var_dump($port8000->getPort());
var_dump($noPort->getPort());

var_dump((string)$port80);
var_dump((string)$noPort);
?>
--EXPECT--
NULL
int(80)
int(8000)
NULL
string(21) "http://example.com:80"
string(18) "http://example.com"