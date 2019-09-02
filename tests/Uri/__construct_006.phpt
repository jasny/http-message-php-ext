--TEST--
Create Uri with a domain name
--FILE--
<?php
var_dump(new HttpMessage\Uri('www.example.com'));
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
  string(15) "www.example.com"
  ["query":"HttpMessage\Uri":private]=>
  string(0) ""
  ["fragment":"HttpMessage\Uri":private]=>
  string(0) ""
}