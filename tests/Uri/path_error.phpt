--TEST--
Uri::withPath() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withPath(['path' => '/foo']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withPath();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withPath() expects parameter 1 to be string, array given
HttpMessage\Uri::withPath() expects exactly 1 parameter, 0 given
