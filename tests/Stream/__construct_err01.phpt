--TEST--
Create Stream with resource
--FILE--
<?php
$resource = fopen('php://memory', 'w+');

$stream = new HttpMessage\Stream($resource);
var_dump($stream);
var_dump($stream->detach() === $resource);
?>
--EXPECTF--
object(HttpMessage\Stream)#1 (1) {
  ["stream":protected]=>
  resource(%d) of type (stream)
}
bool(true)