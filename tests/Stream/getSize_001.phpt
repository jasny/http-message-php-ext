--TEST--
Stream::getSize()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->getSize());

fwrite($resource, "abcdef");
var_dump($stream->getSize());

fwrite($resource, "ghi");
var_dump($stream->getSize());

fseek($resource, 0);
var_dump($stream->getSize());

fwrite($resource, "1234567890abcdef");
var_dump($stream->getSize());

fclose($resource);
var_dump($stream->getSize());
?>
--EXPECT--
int(0)
int(6)
int(9)
int(9)
int(16)
NULL
