--TEST--
Message body
--FILE--
<?php
$response = new HttpMessage\Response();

$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);
$newResponse = $response->withBody($stream);

var_dump(get_class($response->getBody()));
var_dump($response->getBody()->getMetadata('uri'));

var_dump(get_class($newResponse->getBody()));
var_dump($newResponse->getBody() === $stream);
var_dump($newResponse->getBody()->getMetadata('uri'));

?>
--EXPECT--
string(18) "HttpMessage\Stream"
string(10) "php://temp"
string(18) "HttpMessage\Stream"
bool(true)
string(12) "php://memory"