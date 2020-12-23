--TEST--
Stream::read() error: no arguments
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

try {
    $stream->read();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Stream::read() expects exactly 1 %s, 0 given
