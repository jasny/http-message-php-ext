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
--EXPECT--
HttpMessage\UploadedFile::__construct() expects parameter 1 to be a string or object that implements Psr\Http\Message\StreamInterface, array given
HttpMessage\UploadedFile::__construct() expects parameter 1 to be a string or object that implements Psr\Http\Message\StreamInterface, object given
Argument 2 passed to HttpMessage\UploadedFile::__construct() must be of the type int or null, array given
Argument 3 passed to HttpMessage\UploadedFile::__construct() must be of the type int, array given
Argument 4 passed to HttpMessage\UploadedFile::__construct() must be of the type string or null, array given
Argument 5 passed to HttpMessage\UploadedFile::__construct() must be of the type string or null, array given