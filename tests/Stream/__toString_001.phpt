--TEST--
Cast Stream to string
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fwrite($resource, "abcdefghijklmnopqrstuvwxyz");

$stream = new HttpMessage\Stream($resource);

var_dump((string)$stream);
var_dump((string)$stream);

fread($resource, 10);
var_dump((string)$stream);
?>
--EXPECT--
string(26) "abcdefghijklmnopqrstuvwxyz"
string(26) "abcdefghijklmnopqrstuvwxyz"
string(26) "abcdefghijklmnopqrstuvwxyz"
