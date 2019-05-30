--TEST--
Stream::getMetadata() with key
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->getMetadata('uri'));
var_dump($stream->getMetadata('blocked'));

var_dump($stream->getMetadata('not_exist'));

?>
--EXPECT--
string(12) "php://memory"
bool(true)
NULL
