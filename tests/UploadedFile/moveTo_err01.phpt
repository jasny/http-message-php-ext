--TEST--
UploadedFile::moveTo() without a file
--FILE--
<?php
$upload = new HttpMessage\UploadedFile(null, null, UPLOAD_ERR_CANT_WRITE);

try {
    $upload->moveTo(sys_get_temp_dir() . '/movedfile');
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
No file was uploaded or uploaded file not available