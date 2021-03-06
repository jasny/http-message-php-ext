--TEST--
Stream::seek() error: invalid arguments
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

try {
    $stream->seek('hello');
} catch (Error $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    $stream->seek(1, 'hello');
} catch (Error $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}
?>
--EXPECTF--
HttpMessage\Stream::seek()%s int, string given
HttpMessage\Stream::seek()%s int, string given
