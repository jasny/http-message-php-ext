--TEST--
UploadedFile::moveTo() invalid target
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path);

try {
    $upload->moveTo(sys_get_temp_dir() . '/nosuchdir/movedfile');
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--CLEANUP--
<?php
if (file_exists(sys_get_temp_dir() . '/uploadedfile') {
    unlink(sys_get_temp_dir() . '/uploadedfile');
}
?>
--EXPECTF--
Warning: HttpMessage\UploadedFile::moveTo(/tmp/nosuchdir/movedfile): failed to open stream: No such file or directory in %s/moveTo_err03.php on line %d
Failed to move uploaded file '/tmp/uploadedfile' to '/tmp/nosuchdir/movedfile'