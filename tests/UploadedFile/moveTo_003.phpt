--TEST--
UploadedFile::moveTo() with stream
--FILE--
<?php
$resource = fopen('php://temp', 'r+');
fwrite($resource, 'foo');

$stream = new HttpMessage\Stream($resource);
$upload = new HttpMessage\UploadedFile($stream);

$target = sys_get_temp_dir() . '/movedfile';
$upload->moveTo($target);

var_dump(file_get_contents($target));

?>
--CLEAN--
<?php
if (file_exists(sys_get_temp_dir() . '/movedfile') unlink(sys_get_temp_dir() . '/movedfile');
?>
--EXPECT--
string(3) "foo"