--TEST--
Stream::getContents() with non-readable stream
--FILE--
<?php
$resource = fopen(sys_get_temp_dir() . '/nonreadable.tmp', "a");
$stream = new HttpMessage\Stream($resource);

try {
    $stream->getContents();
} catch (RuntimeException $e) {
    echo $e->getMessage();
}
?>
--CLEAN--
<?php
unlink(sys_get_temp_dir() . '/nonreadable.tmp');
?>
--EXPECT--
Stream not readable
