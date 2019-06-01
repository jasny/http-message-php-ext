--TEST--
Stream::getContents()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abcdefghijklmnopqrstuvwxyz");
fseek($resource, 10);

$stream = new HttpMessage\Stream($resource);

var_dump($stream->getContents());
var_dump($stream->getContents());
?>
--EXPECT--
string(16) "klmnopqrstuvwxyz"
string(0) ""
