--TEST--
Stream::read() with non-readable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen(__FILE__, 'r'));

try {
    $stream->write('hello');
} catch (RuntimeException $e) {
    echo $e->getMessage();
}
?>
--EXPECT--
Stream not writable
