--TEST--
Create Uri with a relative path
--FILE--
<?php
var_dump(new HttpMessage\Uri('foo'));
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
  string(3) "foo"
  ["query":protected]=>
  string(0) ""
  ["fragment":protected]=>
  string(0) ""
}
