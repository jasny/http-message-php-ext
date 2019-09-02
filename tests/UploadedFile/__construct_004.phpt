--TEST--
Create UploadedFile with only an error
--FILE--
<?php
$upload = new HttpMessage\UploadedFile(null, null, UPLOAD_ERR_NO_FILE, null, null);

var_dump($upload);
var_dump($upload->getSize());
var_dump($upload->getError());
var_dump($upload->getClientFilename());
var_dump($upload->getClientMediaType());

?>
--EXPECTF--
object(HttpMessage\UploadedFile)#%d (8) {
  ["stream":"HttpMessage\UploadedFile":private]=>
  NULL
  ["file":"HttpMessage\UploadedFile":private]=>
  NULL
  ["size":"HttpMessage\UploadedFile":private]=>
  NULL
  ["error":"HttpMessage\UploadedFile":private]=>
  int(4)
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
int(4)
NULL
NULL