--TEST--
Stream::isSeekable() with detached stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen('php://memory', 'r'));
var_dump($stream->isSeekable());

$stream->detach();
var_dump($stream->isSeekable());
?>
--EXPECT--
bool(true)
bool(false)
