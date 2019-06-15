--TEST--
Message body
--FILE--
<?php
$response = new HttpMessage\Response();
var_dump(get_class($response->getBody()));

$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

$newResponse = $response->withBody($stream);
var_dump(get_class($newResponse->getBody()));
var_dump($newResponse->getBody() === $stream);

?>
--EXPECT--
string(18) "HttpMessage\Stream"
string(18) "HttpMessage\Stream"
bool(true)