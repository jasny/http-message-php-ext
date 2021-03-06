--TEST--
Factory::createStreamFromFile()
--FILE--
<?php
$factory = new HttpMessage\Factory();
$stream = $factory->createStreamFromFile(__FILE__);

var_dump($stream);

$resource = $stream->detach();
var_dump(stream_get_meta_data($resource));

fseek($resource, 0);
var_dump(fread($resource, 16));

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
  string(%d) "%screateStreamFromFile_001.php"
}
string(16) "<?php
$factory ="