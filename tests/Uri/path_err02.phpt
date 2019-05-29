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
--EXPECT--
HttpMessage\Uri::withPath() expects exactly 1 parameter, 0 given
