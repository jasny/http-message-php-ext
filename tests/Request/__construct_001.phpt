--TEST--
Create a Request
--FILE--
<?php
$request = new HttpMessage\Request();

var_dump($request);
var_dump($request->getRequestTarget());
var_dump((string)$request->getUri());
var_dump($request->getBody()->getMetadata('uri'));

?>
--EXPECTF--
object(HttpMessage\Request)#1 (6) {
  ["protocolVersion":protected]=>
  string(3) "1.1"
  ["headers":protected]=>
  array(0) {
  }
  ["body":protected]=>
  object(HttpMessage\Stream)#2 (1) {
    ["stream":protected]=>
    resource(%d) of type (stream)
  }
  ["requestTarget":protected]=>
  NULL
  ["method":protected]=>
  string(0) ""
  ["uri":protected]=>
  object(HttpMessage\Uri)#3 (7) {
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
}
string(1) "/"
string(0) ""
string(10) "php://temp"