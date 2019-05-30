--TEST--
Stream::isWritable() with non-writable stream
--FILE--
<?php
$resource = fopen(__FILE__, 'r');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->isWritable());
?>
--EXPECT--
bool(false)
