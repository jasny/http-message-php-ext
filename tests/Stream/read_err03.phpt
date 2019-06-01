--TEST--
Stream::read() with non-readable stream
--FILE--
<?php
$resource = fopen(sys_get_temp_dir() . '/nonreadable.tmp', "a");
$stream = new HttpMessage\Stream($resource);

try {
    $stream->read(1);
} catch (RuntimeException $e) {
    echo $e->getMessage();
}
?>
--CLEANUP--
<?php
unlink(sys_get_temp_dir() . '/nonreadable.tmp');
?>
--EXPECT--
Stream not readable
