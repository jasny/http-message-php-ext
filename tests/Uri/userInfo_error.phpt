--TEST--
Uri::withUserInfo() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withUserInfo(['userInfo' => 'user']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withUserInfo();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withUserInfo() expects parameter 1 to be string, array given
HttpMessage\Uri::withUserInfo() expects exactly 1 parameter, 0 given
