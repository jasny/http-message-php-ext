--TEST--
Stream::eof()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->eof());

$stream->detach();
var_dump($stream->eof());

?>
--EXPECT--
bool(false)
bool(true)
