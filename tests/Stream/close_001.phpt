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
--EXPECT--
object(HttpMessage\Stream)#1 (1) {
  ["stream":protected]=>
  resource(5) of type (Unknown)
}
string(7) "Unknown"
