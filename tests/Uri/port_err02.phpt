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
--EXPECT--
HttpMessage\Uri::withPort() expects exactly 1 parameter, 0 given
