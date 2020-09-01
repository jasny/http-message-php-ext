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
--EXPECTF--
%sttpMessage\Factory::createUploadedFile()%s Psr\Http\Message\StreamInterface, string given
%sttpMessage\Factory::createUploadedFile()%s Psr\Http\Message\StreamInterface,%sstdClass given
%sttpMessage\Factory::createUploadedFile()%s, array given
%sttpMessage\Factory::createUploadedFile()%sint, array given
%sttpMessage\Factory::createUploadedFile()%s, array given
%sttpMessage\Factory::createUploadedFile()%s, array given
