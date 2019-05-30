--TEST--
Stream::getMetadata()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

var_dump($stream->getMetadata());

?>
--EXPECT--
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
