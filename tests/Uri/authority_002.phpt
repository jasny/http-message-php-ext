--TEST--
Uri::getAuthority()
--FILE--
<?php
$uri = new HttpMessage\Uri();
var_dump($uri->getAuthority());
?>
--EXPECT--
string(0) ""
