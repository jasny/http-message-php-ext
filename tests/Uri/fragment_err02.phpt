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
--EXPECTF--
HttpMessage\Uri::withFragment() expects exactly 1 %s, 0 given
