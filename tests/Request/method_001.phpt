--TEST--
Request::withMethod()
--FILE--
<?php
$request = new HttpMessage\Request();
$getRequest = $request->withMethod('GET');
$postRequest = $getRequest->withMethod('POST');

var_dump($request->getMethod());
var_dump($getRequest->getMethod());
var_dump($postRequest->getMethod());

?>
--EXPECT--
string(0) ""
string(3) "GET"
string(4) "POST"