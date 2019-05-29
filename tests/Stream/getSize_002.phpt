--TEST--
Stream::getSize() with detached stream
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

fwrite($resource, "abcdef");
var_dump($stream->getSize());

$stream->detach();
var_dump($stream->getSize());
?>
--EXPECT--
int(6)
NULL
