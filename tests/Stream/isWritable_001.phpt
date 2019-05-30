--TEST--
Stream::isWritable() with writable stream
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->isWritable());
?>
--EXPECT--
bool(true)
