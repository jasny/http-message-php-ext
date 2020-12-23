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
--EXPECTF--
HttpMessage\Request::withUri() expects exactly 1 %s, 0 given
%sttpMessage\Request::withUri()%sPsr\Http\Message\UriInterface, string given
