--TEST--
Stream::write()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');

$stream = new HttpMessage\Stream($resource);

$stream->write('hello');
$stream->write(' world');

fseek($resource, 0);
var_dump(fread($resource, 100));
?>
--EXPECT--
string(11) "hello world"
