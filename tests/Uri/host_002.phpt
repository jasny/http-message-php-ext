--TEST--
Uri::withHost()
--FILE--
<?php
$blank = new HttpMessage\Uri();
$dotCom = $blank->withHost("www.example.com");
$dotNet = $dotCom->withHost("www.example.net");

var_dump($blank->getHost());
var_dump($dotCom->getHost());
var_dump($dotNet->getHost());
?>
--EXPECT--
string(0) ""
string(15) "www.example.com"
string(15) "www.example.net"
