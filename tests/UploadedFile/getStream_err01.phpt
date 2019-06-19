--TEST--
UploadedFile::getStream() without a file
--FILE--
<?php
$upload = new HttpMessage\UploadedFile(null, null, UPLOAD_ERR_CANT_WRITE);

try {
    $upload->getStream();
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
No file was uploaded or uploaded file not available