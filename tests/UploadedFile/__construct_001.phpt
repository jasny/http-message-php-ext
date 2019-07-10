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
object(HttpMessage\UploadedFile)#1 (8) {
  ["stream":protected]=>
  NULL
  ["file":protected]=>
  string(%d) "%s/uploadedfile"
  ["size":protected]=>
  int(99)
  ["error":protected]=>
  int(0)
  ["clientFilename":protected]=>
  string(10) "myfile.txt"
  ["clientMediaType":protected]=>
  string(10) "text/plain"
  ["moved":protected]=>
  bool(false)
  ["checkUploaded":protected]=>
  bool(false)
}
int(99)
int(0)
string(10) "myfile.txt"
string(10) "text/plain"