--TEST--
Create Uri with a full url
--FILE--
<?php
var_dump(new HttpMessage\Uri('http://acme@www.example.com:8000/foo?answer=42#question'));
?>
--EXPECT--
object(HttpMessage\Uri)#1 (7) {
  ["scheme":protected]=>
  string(4) "http"
  ["userInfo":protected]=>
  string(4) "acme"
  ["host":protected]=>
  string(15) "www.example.com"
  ["port":protected]=>
  int(8000)
  ["path":protected]=>
  string(4) "/foo"
  ["query":protected]=>
  string(9) "answer=42"
  ["fragment":protected]=>
  string(8) "question"
}
