--TEST--
Uri::withUserInfo() error: missing argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withUserInfo();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withUserInfo() expects exactly 1 %s, 0 given
