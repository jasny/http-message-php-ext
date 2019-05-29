--TEST--
Uri::withUserInfo() error: invalid argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withUserInfo(['userInfo' => 'user']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withUserInfo() expects parameter 1 to be string, array given
