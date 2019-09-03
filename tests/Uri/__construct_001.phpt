--TEST--
Create Uri without arguments
--FILE--
<?php
var_dump(new HttpMessage\Uri());
?>
--EXPECTF--
object(HttpMessage\Uri)#%d (7) {
  ["scheme":"HttpMessage\Uri":private]=>
  string(0) ""
  ["userInfo":"HttpMessage\Uri":private]=>
  string(0) ""
  ["host":"HttpMessage\Uri":private]=>
  string(0) ""
  ["port":"HttpMessage\Uri":private]=>
  NULL
  ["path":"HttpMessage\Uri":private]=>
  string(0) ""
  ["query":"HttpMessage\Uri":private]=>
  string(0) ""
  ["fragment":"HttpMessage\Uri":private]=>
  string(0) ""
}