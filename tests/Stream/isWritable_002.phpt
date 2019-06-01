--TEST--
Stream::isWritable() with non-writable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen(__FILE__, 'r'));
var_dump($stream->isWritable());
?>
--EXPECT--
bool(false)
