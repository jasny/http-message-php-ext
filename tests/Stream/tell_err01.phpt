--TEST--
Stream::tell() error: detached stream
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

$stream->detach();

try {
    $stream->tell();
} catch (RuntimeException $e) {
    echo $e->getMessage();
}
?>
--EXPECT--
The stream has been detached
