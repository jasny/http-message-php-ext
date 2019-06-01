--TEST--
Stream::read()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abcdefghijklmnopqrstuvwxyz");
fseek($resource, 0);

$stream = new HttpMessage\Stream($resource);

var_dump($stream->read(10));
var_dump($stream->read(10));
var_dump($stream->read(10));
var_dump($stream->read(10));
?>
--EXPECT--
string(10) "abcdefghij"
string(10) "klmnopqrst"
string(6) "uvwxyz"
string(0) ""
