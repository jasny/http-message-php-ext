--TEST--
UploadedFile::moveTo() with is_uploaded check
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path, null, UPLOAD_ERR_OK, null, null, true);

try {
    $upload->moveTo(sys_get_temp_dir() . '/movedfile');
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
?>
--EXPECTF--
Won't move '/tmp/uploadedfile'; not an uploaded file