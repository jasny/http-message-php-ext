--TEST--
Response::withStatus() with custom reason phrase
--FILE--
<?php
$response = new HttpMessage\Response();

$newResponse = $response->withStatus(200, "ALL CLEAR");
var_dump($newResponse->getStatusCode());
var_dump($newResponse->getReasonPhrase());

?>
--EXPECT--
int(200)
string(9) "ALL CLEAR"