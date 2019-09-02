--TEST--
Create Uri with a relative path
--FILE--
<?php
var_dump(new HttpMessage\Uri('foo'));
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
  string(3) "foo"
  ["query":"HttpMessage\Uri":private]=>
  string(0) ""
  ["fragment":"HttpMessage\Uri":private]=>
  string(0) ""
}