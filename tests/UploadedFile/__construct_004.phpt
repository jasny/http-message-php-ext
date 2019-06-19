--TEST--
Create UploadedFile with only a path
--FILE--
<?php
$upload = new HttpMessage\UploadedFile(null, null, UPLOAD_ERR_NO_FILE, null, null);

var_dump($upload);
var_dump($upload->getSize());
var_dump($upload->getError());
var_dump($upload->getClientFilename());
var_dump($upload->getClientMediaType());

?>
--EXPECT--
object(HttpMessage\UploadedFile)#1 (8) {
  ["stream":protected]=>
  NULL
  ["file":protected]=>
  NULL
  ["size":protected]=>
  NULL
  ["error":protected]=>
  int(4)
  ["clientFilename":protected]=>
  NULL
  ["clientMediaType":protected]=>
  NULL
  ["moved":protected]=>
  bool(false)
  ["checkUploaded":protected]=>
  bool(false)
}
NULL
int(4)
NULL
NULL