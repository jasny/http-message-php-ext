--TEST--
Uri::withFragment() error: missing argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withFragment();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withFragment() expects exactly 1 parameter, 0 given
