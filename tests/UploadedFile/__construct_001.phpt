--TEST--
Create UploadedFile
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path, 99, UPLOAD_ERR_OK, 'myfile.txt', 'text/plain');

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
  string(%d) "%s/uploadedfile"
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