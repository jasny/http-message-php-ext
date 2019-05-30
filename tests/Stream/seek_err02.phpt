--TEST--
Stream::seek() error: invalid arguments
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

try {
    $stream->seek('hello');
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}

try {
    $stream->seek(1, 'hello');
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Stream::seek() expects parameter 1 to be integer, string given
HttpMessage\Stream::seek() expects parameter 2 to be integer, string given
