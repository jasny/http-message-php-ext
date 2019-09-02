--TEST--
Create UploadedFile with path and error
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path, NULL, UPLOAD_ERR_INI_SIZE);

var_dump($upload);
var_dump($upload->getSize());
var_dump($upload->getError());
var_dump($upload->getClientFilename());
var_dump($upload->getClientMediaType());

?>
--CLEAN--
<?php
unlink(sys_get_temp_dir() . '/uploadedfile');
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
  int(1)
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
int(1)
NULL
NULL