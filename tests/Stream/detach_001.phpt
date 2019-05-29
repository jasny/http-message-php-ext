--TEST--
Stream::detach()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

$ret = $stream->detach();
var_dump($stream);

var_dump($ret === $resource);
var_dump(get_resource_type($resource));

var_dump($stream->detach());

$stream->close(); // Calling close() while detached should be ignored
?>
--EXPECT--
object(HttpMessage\Stream)#1 (1) {
  ["stream":protected]=>
  NULL
}
bool(true)
string(6) "stream"
NULL
