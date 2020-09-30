--TEST--
Uri::withPort() error: missing argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withPort();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withPort() expects exactly 1 %s, 0 given
