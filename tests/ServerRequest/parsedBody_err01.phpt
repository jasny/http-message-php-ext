--TEST--
ServerRequest::withParsedBody() with invalid arguments
--FILE--
<?php
$request = new HttpMessage\ServerRequest();

try {
    $request->withQueryParams();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request->withParsedBody('foo=bar;');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\ServerRequest::withQueryParams() expects exactly 1 parameter, 0 given
HttpMessage\ServerRequest::withParsedBody() expects parameter 1 to be array, string given