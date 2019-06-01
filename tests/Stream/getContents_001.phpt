--TEST--
Stream::getContents()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abcdefghijklmnopqrstuvwxyz");
fseek($resource, 0);

$stream = new HttpMessage\Stream($resource);

var_dump($stream->getContents());
var_dump($stream->getContents());
?>
--EXPECT--
string(26) "abcdefghijklmnopqrstuvwxyz"
string(0) ""
