--TEST--
UploadedFile::getStream() after move
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path);

$target = sys_get_temp_dir() . '/movedfile';
$upload->moveTo($target);

var_dump(file_get_contents($target));

?>
--CLEAN--
<?php
if (file_exists(sys_get_temp_dir() . '/uploadedfile') {
    unlink(sys_get_temp_dir() . '/uploadedfile');
}
if (file_exists(sys_get_temp_dir() . '/moved') {
    unlink(sys_get_temp_dir() . '/moved');
}
?>
--EXPECT--
string(3) "foo"