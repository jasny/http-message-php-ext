--TEST--
Response::withStatus()
--FILE--
<?php
$response = new HttpMessage\Response();

$newResponse = $response->withStatus(200);
var_dump($newResponse->getStatusCode());
var_dump($newResponse->getReasonPhrase());

?>
--EXPECT--
int(200)
string(2) "OK"