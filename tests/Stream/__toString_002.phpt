--TEST--
Cast unreadable Stream to string from non-readable stream
--FILE--
<?php
$resource = fopen(sys_get_temp_dir() . '/nonreadable.tmp', "a");
$stream = new HttpMessage\Stream($resource);

var_dump((string)$stream);
?>
--CLEAN--
<?php
unlink(sys_get_temp_dir() . '/nonreadable.tmp');
?>
--EXPECT--
string(0) ""
