--TEST--
UploadedFile::moveTo() with non-existing file
--FILE--
<?php
$upload = new HttpMessage\UploadedFile(__DIR__ . '/not/exist');

try {
    $upload->moveTo(sys_get_temp_dir() . '/movedfile');
} catch (RuntimeException $e) {
    echo strtr($e->getMessage(), [DIRECTORY_SEPARATOR => '/']), "\n";
}

?>
--CLEAN--
<?php
if (file_exists(sys_get_temp_dir() . '/movedfile') {
    unlink(sys_get_temp_dir() . '/movedfile');
}
?>
--EXPECTF--
Warning: HttpMessage\UploadedFile::moveTo(%s): %sailed to open stream: No such file or directory in %s on line %d
Failed to move uploaded file '%s/UploadedFile/not/exist' to '%s/movedfile'
