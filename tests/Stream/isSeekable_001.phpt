--TEST--
Stream::isSeekable()
--FILE--
<?php
$seekableStream = new HttpMessage\Stream(fopen('php://memory', 'r'));
var_dump($seekableStream->isSeekable());

$unseekableStream = new HttpMessage\Stream(fopen('php://stdin', 'r'));
var_dump($unseekableStream->isSeekable());
?>
--EXPECT--
bool(true)
bool(false)
