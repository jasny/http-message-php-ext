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
--EXPECT--
HttpMessage\Uri::withUserInfo() expects exactly 1 parameter, 0 given
