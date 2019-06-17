--TEST--
Request::withUri()
--FILE--
<?php
$uri = new HttpMessage\Uri();

$request = (new HttpMessage\Request())
    ->withUri($uri);

var_dump($request->getUri() === $uri);

?>
--EXPECT--
bool(true)