--TEST--
Stream::rewind()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abc");

$stream = new HttpMessage\Stream($resource);

var_dump(fread($resource, 4));

$stream->rewind();
var_dump(fread($resource, 4));
?>
--EXPECT--
string(0) ""
string(3) "abc"
