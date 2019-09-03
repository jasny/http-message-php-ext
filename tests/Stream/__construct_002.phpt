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
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}
bool(true)