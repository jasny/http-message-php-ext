--TEST--
Create Uri with username and password
--FILE--
<?php
var_dump(new HttpMessage\Uri('https://acme:secure@www.example.com'));
?>
--EXPECT--
object(HttpMessage\Uri)#1 (7) {
  ["scheme":protected]=>
  string(5) "https"
  ["userInfo":protected]=>
  string(11) "acme:secure"
  ["host":protected]=>
  string(15) "www.example.com"
  ["port":protected]=>
  NULL
  ["path":protected]=>
  string(0) ""
  ["query":protected]=>
  string(0) ""
  ["fragment":protected]=>
  string(0) ""
}
