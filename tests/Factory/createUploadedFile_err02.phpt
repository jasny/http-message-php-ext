--TEST--
Factory::createUploadedFile() with non-readable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(sys_get_temp_dir() . '/uploadedfile', 'w');
$factory = new HttpMessage\Factory();

try {
    $factory->createUploadedFile($stream);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
--CLEAN--
<?php
if (file_exists(sys_get_temp_dir() . '/uploadedfile')) unlink(sys_get_temp_dir() . '/uploadedfile');
?>
--EXPECT--
Stream provided for uploaded file is not readable