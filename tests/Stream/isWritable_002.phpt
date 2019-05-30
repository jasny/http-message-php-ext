--TEST--
Stream::isWritable() with non-writable stream
--FILE--
<?php
$fp = fopen(sys_get_temp_dir() . '/isreadable.tmp', "a");

$stream = new HttpMessage\Stream($fp);
var_dump($stream->isWritable());
?>
--EXPECT--
bool(true)
