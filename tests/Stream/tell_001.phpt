--TEST--
Stream::tell()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->tell());

fwrite($resource, "abcdef");
var_dump($stream->tell());

fwrite($resource, "ghi");
var_dump($stream->tell());

fseek($resource, 0);
var_dump($stream->tell());

fwrite($resource, "1234567890abcdef");
var_dump($stream->tell());
?>
--EXPECT--
int(0)
int(6)
int(9)
int(0)
int(16)
