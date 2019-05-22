--TEST--
Create Uri without arguments
--FILE--
<?php
var_dump(new HttpMessage\Uri());
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
  string(0) ""
  ["query":protected]=>
  string(0) ""
  ["fragment":protected]=>
  string(0) ""
}
