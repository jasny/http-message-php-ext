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
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (Unknown)
}
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (Unknown)
}