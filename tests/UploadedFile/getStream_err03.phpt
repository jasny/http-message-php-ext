--TEST--
UploadedFile::getStream() with non-existing file
--FILE--
<?php
$upload = new HttpMessage\UploadedFile(__DIR__ . '/not/exist');

try {
    $upload->getStream();
} catch (RuntimeException $e) {
    echo strtr($e->getMessage(), [DIRECTORY_SEPARATOR => '/']), "\n";
}
?>
--EXPECTF--
Failed to open '%s/UploadedFile/not/exist' stream