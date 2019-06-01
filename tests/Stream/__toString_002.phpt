--TEST--
Cast unreadable Stream to string
--FILE--
<?php
$resource = fopen(sys_get_temp_dir() . '/nonreadable.tmp', "a");
$stream = new HttpMessage\Stream($resource);

var_dump((string)$stream);
?>
--CLEANUP--
<?php
unlink(sys_get_temp_dir() . '/nonreadable.tmp');
?>
--EXPECT--
string(0) ""
