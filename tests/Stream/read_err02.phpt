--TEST--
Stream::read() err: invalid arguments
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

try {
    $stream->read('hello');
} catch (Error $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    $stream->read(-1);
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Stream::read() expects parameter 1 to be int, string given
Length parameter must be equal or greater than 0
