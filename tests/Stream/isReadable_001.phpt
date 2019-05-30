--TEST--
Stream::isReadable() with readable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen(__FILE__, 'r'));
var_dump($stream->isReadable());
?>
--EXPECT--
bool(true)
