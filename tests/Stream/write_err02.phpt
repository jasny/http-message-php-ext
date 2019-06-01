--TEST--
Stream::read() err: invalid arguments
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

try {
    $stream->write(['foo' => 'bar']);
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Stream::write() expects parameter 1 to be string, array given