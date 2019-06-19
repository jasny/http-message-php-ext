--TEST--
UploadedFile::getStream() after move
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path);
$upload->moveTo(sys_get_temp_dir() . '/movedfile');

try {
    $upload->getStream();
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--CLEANUP--
<?php
if (file_exists(sys_get_temp_dir() . '/uploadedfile') {
    unlink(sys_get_temp_dir() . '/uploadedfile');
}
if (file_exists(sys_get_temp_dir() . '/movedfile') {
    unlink(sys_get_temp_dir() . '/movedfile');
}
?>
--EXPECT--
Uploaded file '/tmp/uploadedfile' has already been moved
