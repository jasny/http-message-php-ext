--TEST--
Cast unreadable Stream to string from file
--FILE--
<?php
$path = sys_get_temp_dir() . '/nonreadable.tmp';
file_put_contents($path, "foo");

$resource = fopen($path, 'r');
$stream = new HttpMessage\Stream($resource);

var_dump((string)$stream);

?>
--CLEAN--
<?php
unlink(sys_get_temp_dir() . '/nonreadable.tmp');
?>
--EXPECT--
string(3) "foo"