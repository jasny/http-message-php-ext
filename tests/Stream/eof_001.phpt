--TEST--
Stream::eof()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->eof());

fread($resource, 1);
var_dump($stream->eof());

fwrite($resource, "abcdef");
var_dump($stream->eof());

fseek($resource, 0);
var_dump($stream->eof());

fread($resource, 3);
var_dump($stream->eof());

fread($resource, 4);
var_dump($stream->eof());
?>
--EXPECT--
bool(false)
bool(true)
bool(true)
bool(false)
bool(false)
bool(true)
