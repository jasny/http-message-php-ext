--TEST--
Create Uri with a full url
--FILE--
<?php
var_dump(new HttpMessage\Uri('http://acme@www.example.com:8000/foo?answer=42#question'));
?>
--EXPECTF--
object(HttpMessage\Uri)#%d (7) {
  ["scheme":"HttpMessage\Uri":private]=>
  string(4) "http"
  ["userInfo":"HttpMessage\Uri":private]=>
  string(4) "acme"
  ["host":"HttpMessage\Uri":private]=>
  string(15) "www.example.com"
  ["port":"HttpMessage\Uri":private]=>
  int(8000)
  ["path":"HttpMessage\Uri":private]=>
  string(4) "/foo"
  ["query":"HttpMessage\Uri":private]=>
  string(9) "answer=42"
  ["fragment":"HttpMessage\Uri":private]=>
  string(8) "question"
}