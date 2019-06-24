--TEST--
Request::withUri()
--FILE--
<?php
$uri = new HttpMessage\Uri();

$request = (new HttpMessage\Request());
$newRequest = $request->withUri($uri);

var_dump($request->getUri() !== $uri);
var_dump($newRequest->getUri() === $uri);

?>
--EXPECT--
bool(true)
bool(true)