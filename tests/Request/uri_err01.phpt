--TEST--
Request::withUri() with invalid arguments
--FILE--
<?php
$request = new HttpMessage\Request();

try {
    $request->withUri();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request->withUri('http://www.example.com');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\Request::withUri() expects exactly 1 parameter, 0 given
Argument 1 passed to HttpMessage\Request::withUri() must implement interface Psr\Http\Message\UriInterface, string given
