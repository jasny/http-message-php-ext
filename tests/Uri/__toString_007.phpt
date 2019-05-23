--TEST--
Cast Uri to string with authority but without schema
--FILE--
<?php
$uri = (new HttpMessage\Uri)->withHost('www.example.com');

var_dump((string)$uri);
var_dump((string)($uri->withUserInfo('user:pass')));
?>
--EXPECT--
string(17) "//www.example.com"
string(27) "//user:pass@www.example.com"
