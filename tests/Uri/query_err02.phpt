--TEST--
Uri::withQuery() error: missing argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withQuery();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withQuery() expects exactly 1 %s, 0 given
