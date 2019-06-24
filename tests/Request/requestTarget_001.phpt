--TEST--
Request::withRequestTarget()
--FILE--
<?php
$request = new HttpMessage\Request();
$newRequest = $request->withRequestTarget('/foo');

var_dump($request->getRequestTarget());
var_dump($newRequest->getRequestTarget());

?>
--EXPECT--
string(1) "/"
string(4) "/foo"