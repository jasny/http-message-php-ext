--TEST--
Create Uri with a domain name
--FILE--
<?php
var_dump(new HttpMessage\Uri('www.example.com'));
?>
--EXPECT--
object(HttpMessage\Uri)#1 (7) {
  ["scheme":protected]=>
  string(0) ""
  ["userInfo":protected]=>
  string(0) ""
  ["host":protected]=>
  string(0) ""
  ["port":protected]=>
  NULL
  ["path":protected]=>
  string(15) "www.example.com"
  ["query":protected]=>
  string(0) ""
  ["fragment":protected]=>
  string(0) ""
}