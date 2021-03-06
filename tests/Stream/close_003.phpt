--TEST--
Stream::close()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
$stream = new HttpMessage\Stream($resource);

$stream->close();

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

try {
    $stream->read(1);
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

try {
    $stream->getContents();
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

try {
    $stream->write("abc");
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

var_dump((string)$stream);
var_dump($stream->getSize());

var_dump($stream->eof());
var_dump($stream->isSeekable());
var_dump($stream->isWritable());
var_dump($stream->isReadable());

var_dump($stream->getMetadata());
var_dump($stream->getMetadata('uri'));
?>
--EXPECT--
Stream is closed
Stream is closed
Stream is closed
Stream is closed
Stream is closed
Stream is closed
string(0) ""
NULL
bool(true)
bool(false)
bool(false)
bool(false)
array(0) {
}
NULL
