--TEST--
Stream::tell() error: closed stream
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

fclose($resource);

try {
    $stream->tell();
} catch (RuntimeException $e) {
    echo $e->getMessage();
}
?>
--EXPECT--
The stream has been closed
