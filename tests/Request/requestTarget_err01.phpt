--TEST--
Request::withRequestTarget() with invalid arguments
--FILE--
<?php
$request = new HttpMessage\Request();

try {
    $request->withRequestTarget();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request->withRequestTarget(['foo', 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\Request::withRequestTarget() expects exactly 1 parameter, 0 given
HttpMessage\Request::withRequestTarget() expects parameter 1 to be string, array given
