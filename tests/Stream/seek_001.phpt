--TEST--
Stream::seek()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abcdefghijklmnopqrstuvwxyz");

$stream = new HttpMessage\Stream($resource);

$stream->seek(10);
var_dump(fread($resource, 64));

$stream->seek(3, SEEK_SET);
var_dump(fread($resource, 6));

$stream->seek(4, SEEK_CUR);
var_dump(fread($resource, 6));

$stream->seek(-3, SEEK_CUR);
var_dump(fread($resource, 6));

$stream->seek(-12, SEEK_END);
var_dump(fread($resource, 6));

$stream->seek(100);
var_dump(fread($resource, 6));
?>
--EXPECT--
string(16) "klmnopqrstuvwxyz"
string(6) "defghi"
string(6) "nopqrs"
string(6) "qrstuv"
string(6) "opqrst"
string(0) ""
