--TEST--
Request::withMethod()
--FILE--
<?php
$request = (new HttpMessage\Request())
    ->withMethod('POST');

var_dump($request->getMethod());

?>
--EXPECT--
string(4) "POST"