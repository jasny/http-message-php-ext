--TEST--
Stream::isSeekable() with seekable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen('php://memory', 'r'));
var_dump($stream->isSeekable());
?>
--EXPECT--
bool(true)
