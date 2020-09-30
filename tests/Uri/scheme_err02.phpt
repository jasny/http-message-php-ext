--TEST--
Uri::withHost() error: missing argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withScheme();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withScheme() expects exactly 1 %s, 0 given
