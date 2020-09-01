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
--EXPECTF--
HttpMessage\Uri::withUserInfo()%s string, array given
