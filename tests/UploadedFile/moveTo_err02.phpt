--TEST--
UploadedFile::moveTo() after move
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path);
$upload->moveTo(sys_get_temp_dir() . '/movedfile');

try {
    $upload->moveTo(sys_get_temp_dir() . '/other');
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--CLEAN--
<?php
if (file_exists(sys_get_temp_dir() . '/uploadedfile') {
    unlink(sys_get_temp_dir() . '/uploadedfile');
}
if (file_exists(sys_get_temp_dir() . '/movedfile') {
    unlink(sys_get_temp_dir() . '/movedfile');
}
if (file_exists(sys_get_temp_dir() . '/other') {
    unlink(sys_get_temp_dir() . '/other');
}
?>
--EXPECTF--
Uploaded file '%s/uploadedfile' has already been moved
