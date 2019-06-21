--TEST--
Request::withRequestTarget() using Uri object
--FILE--
<?php
$request = (new HttpMessage\Request())
    ->withUri(new HttpMessage\Uri('http://www.example.com:80/foo?color=red'));

$fooRequest = $request->withRequestTarget('/bar');
$defaultRequest = $fooRequest->withRequestTarget(null);

var_dump($request->getRequestTarget());
var_dump($fooRequest->getRequestTarget());
var_dump($defaultRequest->getRequestTarget());

?>
--EXPECT--
string(14) "/foo?color=red"
string(4) "/bar"
string(14) "/foo?color=red"