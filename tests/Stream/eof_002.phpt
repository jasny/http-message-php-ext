--TEST--
Stream::eof()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->eof());

fclose($resource);
var_dump($stream->eof());

?>
--EXPECT--
bool(false)
bool(true)
