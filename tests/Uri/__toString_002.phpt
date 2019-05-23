--TEST--
Cast Uri to string without url
--FILE--
<?php
$uri = new HttpMessage\Uri();
var_dump((string)$uri);
?>
--EXPECT--
string(0) ""
