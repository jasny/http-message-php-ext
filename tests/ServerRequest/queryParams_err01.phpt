--TEST--
ServerRequest::withQueryParams() with invalid arguments
--FILE--
<?php
$request = new HttpMessage\ServerRequest();

try {
    $request->withQueryParams();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request->withQueryParams('foo=bar;');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\ServerRequest::withQueryParams() expects exactly 1 parameter, 0 given
Argument 1 passed to HttpMessage\ServerRequest::withQueryParams() must be of the type array, string given