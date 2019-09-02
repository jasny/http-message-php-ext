--TEST--
Create Uri with username and password
--FILE--
<?php
var_dump(new HttpMessage\Uri('https://acme:secure@www.example.com'));
?>
--EXPECTF--
object(HttpMessage\Uri)#%d (7) {
  ["scheme":"HttpMessage\Uri":private]=>
  string(5) "https"
  ["userInfo":"HttpMessage\Uri":private]=>
  string(11) "acme:secure"
  ["host":"HttpMessage\Uri":private]=>
  string(15) "www.example.com"
  ["port":"HttpMessage\Uri":private]=>
  NULL
  ["path":"HttpMessage\Uri":private]=>
  string(0) ""
  ["query":"HttpMessage\Uri":private]=>
  string(0) ""
  ["fragment":"HttpMessage\Uri":private]=>
  string(0) ""
}