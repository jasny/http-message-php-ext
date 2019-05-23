--TEST--
Uri::withScheme()
--FILE--
<?php
$blank = new HttpMessage\Uri();
$user = $blank->withUserInfo("user");
$userPass = $user->withUserInfo("user:pass");

var_dump($blank->getUserInfo());
var_dump($user->getUserInfo());
var_dump($userPass->getUserInfo());
?>
--EXPECT--
string(0) ""
string(4) "user"
string(9) "user:pass"
