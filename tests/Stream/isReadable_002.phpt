--TEST--
Stream::isReadable() with non-readable stream
--FILE--
<?php
$resource = fopen(sys_get_temp_dir() . '/isreadable.tmp', "a");
$stream = new HttpMessage\Stream($resource);

var_dump($stream->isReadable());
?>
--CLEANUP--
<?php
unlink(sys_get_temp_dir() . '/isreadable.tmp');
?>
--EXPECT--
bool(false)
