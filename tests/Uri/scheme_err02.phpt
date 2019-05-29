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
--EXPECT--
HttpMessage\Uri::withScheme() expects exactly 1 parameter, 0 given
