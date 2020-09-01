--TEST--
ServerRequest::withCookieParams() with invalid arguments
--FILE--
<?php
$request = new HttpMessage\ServerRequest();

try {
    $request->withCookieParams();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request->withCookieParams('foo=bar;');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\ServerRequest::withCookieParams() expects exactly 1 parameter, 0 given
%sttpMessage\ServerRequest::withCookieParams()%s array, string given
