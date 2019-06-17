--TEST--
Request::withRequestTarget() using Uri object
--FILE--
<?php
$request = (new HttpMessage\Request())
    ->withUri(new HttpMessage\Uri('http://www.example.com/foo:80'));

$fooRequest = $request->withRequestTarget('/foo');
$defaultRequest = $fooRequest->withRequestTarget(null);

var_dump($request->getRequestTarget());
var_dump($fooRequest->getRequestTarget());
var_dump($defaultRequest->getRequestTarget());

?>
--EXPECT--
string(1) "/"
string(4) "/foo"
string(1) "/"
