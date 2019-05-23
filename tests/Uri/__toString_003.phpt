--TEST--
Cast Uri to string with basic url
--FILE--
<?php
$uri = new HttpMessage\Uri('https://www.example.com/');
var_dump((string)$uri);
?>
--EXPECT--
string(24) "https://www.example.com/"
