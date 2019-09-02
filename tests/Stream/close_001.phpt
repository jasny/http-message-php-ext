--TEST--
Stream::close()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

$stream->close();
var_dump($stream);

var_dump(get_resource_type($resource));

$stream->close(); // No error
?>
--EXPECTF--
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (Unknown)
}
string(7) "Unknown"