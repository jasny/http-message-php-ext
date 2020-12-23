--TEST--
Stream::write() error: no arguments
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

try {
    $stream->write();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Stream::write() expects exactly 1 %s, 0 given
