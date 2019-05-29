--TEST--
Stream::close() on already closed
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

fclose($resource);
var_dump($stream);

$stream->close();
var_dump($stream);
?>
--EXPECTF--
object(HttpMessage\Stream)#1 (1) {
  ["stream":protected]=>
  resource(%d) of type (Unknown)
}
object(HttpMessage\Stream)#1 (1) {
  ["stream":protected]=>
  resource(%d) of type (Unknown)
}