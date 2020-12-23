--TEST--
Uri::withPath() error: missing argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withPath();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withPath() expects exactly 1 %s, 0 given
