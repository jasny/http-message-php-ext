--TEST--
Stream::seek() error: unseekable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(fopen('php://stdin', 'r'));

try {
    $stream->seek(0);
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
Stream is not seekable