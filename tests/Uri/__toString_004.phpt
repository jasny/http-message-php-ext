--TEST--
Cast Uri to string with only user info and port
--FILE--
<?php
$uri = (new HttpMessage\Uri)
    ->withUserInfo('foo:bar')
    ->withPort(8000);

var_dump((string)$uri);
?>
--EXPECT--
string(0) ""
