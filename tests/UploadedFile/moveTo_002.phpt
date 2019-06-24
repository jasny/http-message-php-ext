--TEST--
UploadedFile::moveTo() after failed move
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path);

try {
    @$upload->moveTo(sys_get_temp_dir() . '/nosuchdir/movedfile');
} catch (RuntimeException $e) {
    // expected
}

$target = sys_get_temp_dir() . '/movedfile';
$upload->moveTo($target);

var_dump(file_get_contents($target));

?>
--CLEAN--
<?php
if (file_exists(sys_get_temp_dir() . '/uploadedfile') {
    unlink(sys_get_temp_dir() . '/uploadedfile');
}
?>
--EXPECT--
string(3) "foo"