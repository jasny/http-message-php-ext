--TEST--
Response::withStatus() with an unknown status code
--FILE--
<?php
$response = new HttpMessage\Response();

$newResponse = $response->withStatus(990);
var_dump($newResponse->getStatusCode());
var_dump($newResponse->getReasonPhrase());

?>
--EXPECT--
int(990)
string(0) ""