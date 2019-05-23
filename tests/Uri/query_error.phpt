--TEST--
Uri::withQuery() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withQuery(['foo' => 1]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withQuery();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withQuery() expects parameter 1 to be string, array given
HttpMessage\Uri::withQuery() expects exactly 1 parameter, 0 given
