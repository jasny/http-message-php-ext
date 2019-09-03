--TEST--
Factory::createUploadedFile()
--FILE--
<?php
$stream = new HttpMessage\Stream();

$factory = new HttpMessage\Factory();
$upload = $factory->createUploadedFile($stream, 99, UPLOAD_ERR_OK, 'myfile.txt', 'text/plain');

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
  int(99)
  ["error":"HttpMessage\UploadedFile":private]=>
  int(0)
  ["clientFilename":"HttpMessage\UploadedFile":private]=>
  string(10) "myfile.txt"
  ["clientMediaType":"HttpMessage\UploadedFile":private]=>
  string(10) "text/plain"
  ["moved":"HttpMessage\UploadedFile":private]=>
  bool(false)
  ["checkUploaded":"HttpMessage\UploadedFile":private]=>
  bool(false)
}
int(99)
int(0)
string(10) "myfile.txt"
string(10) "text/plain"
bool(true)
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}