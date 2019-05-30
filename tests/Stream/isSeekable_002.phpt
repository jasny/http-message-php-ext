--TEST--
Stream::isSeekable() with unseekable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen('php://stdin', 'r'));
var_dump($stream->isSeekable());
?>
--EXPECT--
bool(false)
