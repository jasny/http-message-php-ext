--TEST--
Uri::withFragment() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withFragment(['fragment' => 'foo']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withFragment();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withFragment() expects parameter 1 to be string, array given
HttpMessage\Uri::withFragment() expects exactly 1 parameter, 0 given
