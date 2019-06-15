--TEST--
Message::withProtocolVersion()
--FILE--
<?php
$response11 = new HttpMessage\Response();
$response10 = $response11->withProtocolVersion('1.0');
$response20 = $response10->withProtocolVersion('2.0');

var_dump($response10->getProtocolVersion());
var_dump($response11->getProtocolVersion());
var_dump($response20->getProtocolVersion());

?>
--EXPECT--
string(3) "1.0"
string(3) "1.1"
string(3) "2.0"