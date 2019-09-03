--TEST--
Factory::createUploadedFile() with invalid argument
--FILE--
<?php
$stream = new HttpMessage\Stream();
$factory = new HttpMessage\Factory();

try {
    $factory->createUploadedFile('foo');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $factory->createUploadedFile((object)[]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $factory->createUploadedFile($stream, [], UPLOAD_ERR_OK, 'myfile.txt', 'text/plain');
} catch (TypeError $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    $factory->createUploadedFile($stream, 99, [], 'myfile.txt', 'text/plain');
} catch (TypeError $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    $factory->createUploadedFile($stream, 99, UPLOAD_ERR_OK, [], 'text/plain');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $factory->createUploadedFile($stream, 99, UPLOAD_ERR_OK, 'myfile.txt', []);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Argument 1 passed to HttpMessage\Factory::createUploadedFile() must implement interface Psr\Http\Message\StreamInterface, string given
Argument 1 passed to HttpMessage\Factory::createUploadedFile() must implement interface Psr\Http\Message\StreamInterface, instance of stdClass given
Argument 2 passed to HttpMessage\Factory::createUploadedFile() must be of the type int or null, array given
Argument 3 passed to HttpMessage\Factory::createUploadedFile() must be of the type int, array given
Argument 4 passed to HttpMessage\Factory::createUploadedFile() must be of the type string or null, array given
Argument 5 passed to HttpMessage\Factory::createUploadedFile() must be of the type string or null, array given