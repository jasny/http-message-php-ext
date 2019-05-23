--TEST--
Uri::withHost() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withScheme(['scheme' => 'http']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withScheme();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withScheme() expects parameter 1 to be string, array given
HttpMessage\Uri::withScheme() expects exactly 1 parameter, 0 given
