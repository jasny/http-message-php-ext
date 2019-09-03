--TEST--
Stream::getMetadata()
--FILE--
<?php
$stream = new HttpMessage\Stream(__FILE__, 'r');

var_dump($stream->getMetadata());

?>
--EXPECTF--
array(9) {
  ["timed_out"]=>
  bool(false)
  ["blocked"]=>
  bool(true)
  ["eof"]=>
  bool(false)
  ["wrapper_type"]=>
  string(9) "plainfile"
  ["stream_type"]=>
  string(5) "STDIO"
  ["mode"]=>
  string(1) "r"
  ["unread_bytes"]=>
  int(0)
  ["seekable"]=>
  bool(true)
  ["uri"]=>
  string(%d) "%sgetMetadata_003.php"
}
