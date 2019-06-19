--TEST--
Create UploadedFile with only a path
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path);

var_dump($upload);
var_dump($upload->getSize());
var_dump($upload->getError());
var_dump($upload->getClientFilename());
var_dump($upload->getClientMediaType());

?>
--CLEANUP--
<?php
unlink(sys_get_temp_dir() . '/uploadedfile');
?>
--EXPECT--
object(HttpMessage\UploadedFile)#1 (8) {
  ["stream":protected]=>
  NULL
  ["file":protected]=>
  string(17) "/tmp/uploadedfile"
  ["size":protected]=>
  NULL
  ["error":protected]=>
  int(0)
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
int(0)
NULL
NULL