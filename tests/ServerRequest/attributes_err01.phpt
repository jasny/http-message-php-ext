--TEST--
ServerRequest::withAttribute() with invalid arguments
--FILE--
<?php
$request = new HttpMessage\ServerRequest();

try {
    $request->withAttribute('foo');
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request->withAttribute(['foo' => 'bar'], 22);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\ServerRequest::withAttribute() expects exactly 2 parameters, 1 given
HttpMessage\ServerRequest::withAttribute() expects parameter 1 to be string, array given