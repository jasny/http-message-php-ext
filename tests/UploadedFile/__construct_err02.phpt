--TEST--
Create UploadedFile with a non-readable stream
--FILE--
<?php
$stream = new HttpMessage\Stream(sys_get_temp_dir() . '/uploadedfile', 'w');

try {
    new HttpMessage\UploadedFile($stream);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Stream provided for uploaded file is not readable