--TEST--
Request::withRequestTarget()
--FILE--
<?php
$request = (new HttpMessage\Request())
    ->withRequestTarget('/foo');

var_dump($request->getRequestTarget());

?>
--EXPECT--
string(4) "/foo"