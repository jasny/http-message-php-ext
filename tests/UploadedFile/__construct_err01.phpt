--TEST--
Create UploadedFile with invalid arguments
--FILE--
<?php
$stream = new HttpMessage\Stream();

try {
    new HttpMessage\UploadedFile([]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\UploadedFile((object)[]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\UploadedFile($stream, [], UPLOAD_ERR_OK, 'myfile.txt', 'text/plain');
} catch (TypeError $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    new HttpMessage\UploadedFile($stream, 99, [], 'myfile.txt', 'text/plain');
} catch (TypeError $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    new HttpMessage\UploadedFile($stream, 99, UPLOAD_ERR_OK, [], 'text/plain');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\UploadedFile($stream, 99, UPLOAD_ERR_OK, 'myfile.txt', []);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\UploadedFile::__construct() expects parameter 1 to be a string or object that implements Psr\Http\Message\StreamInterface, array given
HttpMessage\UploadedFile::__construct() expects parameter 1 to be a string or object that implements Psr\Http\Message\StreamInterface, %s given
%sttpMessage\UploadedFile::__construct()%s, array given
%sttpMessage\UploadedFile::__construct()%s, array given
%sttpMessage\UploadedFile::__construct()%s, array given
%sttpMessage\UploadedFile::__construct()%s, array given
