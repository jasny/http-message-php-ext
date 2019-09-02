--TEST--
Factory::createStream() with content
--FILE--
<?php
$factory = new HttpMessage\Factory();
$stream = $factory->createStream("hello");

var_dump($stream);

$resource = $stream->detach();
var_dump(stream_get_meta_data($resource));

fseek($resource, 0);
var_dump(fread($resource, 1024));

?>
--EXPECTF--
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}
array(6) {
  ["wrapper_type"]=>
  string(3) "PHP"
  ["stream_type"]=>
  string(4) "TEMP"
  ["mode"]=>
  string(3) "w+b"
  ["unread_bytes"]=>
  int(0)
  ["seekable"]=>
  bool(true)
  ["uri"]=>
  string(10) "php://temp"
}
string(5) "hello"