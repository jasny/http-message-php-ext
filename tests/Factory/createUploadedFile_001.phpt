--TEST--
Factory::createUploadedFile() with a stream
--FILE--
<?php
$stream = new HttpMessage\Stream();

$factory = new HttpMessage\Factory();
$upload = $factory->createUploadedFile($stream);

var_dump($upload);
var_dump($upload->getSize());
var_dump($upload->getError());
var_dump($upload->getClientFilename());
var_dump($upload->getClientMediaType());

var_dump($upload->getStream() === $stream);
var_dump($upload->getStream());

?>
--EXPECTF--
object(HttpMessage\UploadedFile)#%d (8) {
  ["stream":"HttpMessage\UploadedFile":private]=>
  object(HttpMessage\Stream)#%d (1) {
    ["stream":"HttpMessage\Stream":private]=>
    resource(%d) of type (stream)
  }
  ["file":"HttpMessage\UploadedFile":private]=>
  NULL
  ["size":"HttpMessage\UploadedFile":private]=>
  NULL
  ["error":"HttpMessage\UploadedFile":private]=>
  int(0)
  ["clientFilename":"HttpMessage\UploadedFile":private]=>
  NULL
  ["clientMediaType":"HttpMessage\UploadedFile":private]=>
  NULL
  ["moved":"HttpMessage\UploadedFile":private]=>
  bool(false)
  ["checkUploaded":"HttpMessage\UploadedFile":private]=>
  bool(false)
}
NULL
int(0)
NULL
NULL
bool(true)
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}