--TEST--
Stream::detach() call all methods
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abc");
$stream = new HttpMessage\Stream($resource);

$stream->detach();

try {
    $stream->tell();
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

try {
    $stream->seek(1);
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

try {
    $stream->rewind();
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

var_dump($stream->getSize());

var_dump($stream->eof());
var_dump($stream->isSeekable());
var_dump($stream->isWritable());
var_dump($stream->isReadable());

var_dump($stream->getMetadata());
var_dump($stream->getMetadata('uri'));
?>
--EXPECT--
Stream is detached
Stream is detached
Stream is detached
NULL
bool(true)
bool(false)
bool(false)
bool(false)
array(0) {
}
NULL
