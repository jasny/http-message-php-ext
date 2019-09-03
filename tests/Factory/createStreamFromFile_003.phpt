--TEST--
Factory::createStreamFromFile() with PHP stream
--FILE--
<?php
$factory = new HttpMessage\Factory();
$stream = $factory->createStreamFromFile('php://memory', 'w+');

var_dump($stream);

$resource = $stream->detach();
var_dump(stream_get_meta_data($resource));

?>
--EXPECTF--
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}
array(9) {
  ["timed_out"]=>
  bool(false)
  ["blocked"]=>
  bool(true)
  ["eof"]=>
  bool(false)
  ["wrapper_type"]=>
  string(3) "PHP"
  ["stream_type"]=>
  string(6) "MEMORY"
  ["mode"]=>
  string(3) "w+b"
  ["unread_bytes"]=>
  int(0)
  ["seekable"]=>
  bool(true)
  ["uri"]=>
  string(12) "php://memory"
}