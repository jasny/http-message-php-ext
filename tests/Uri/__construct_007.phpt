--TEST--
Create Uri with an absolute path
--FILE--
<?php
var_dump(new HttpMessage\Uri('/foo'));
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
  string(4) "/foo"
  ["query":protected]=>
  string(0) ""
  ["fragment":protected]=>
  string(0) ""
}