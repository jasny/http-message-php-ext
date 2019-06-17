--TEST--
Create a ServerRequest without any arguments
--FILE--
<?php
$request = new HttpMessage\ServerRequest();

var_dump($request);

?>
--EXPECTF--
object(HttpMessage\ServerRequest)#1 (12) {
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
  ["serverParams":protected]=>
  array(0) {
  }
  ["cookieParams":protected]=>
  array(0) {
  }
  ["queryParams":protected]=>
  array(0) {
  }
  ["uploadedFiles":protected]=>
  array(0) {
  }
  ["parsedBody":protected]=>
  NULL
  ["attributes":protected]=>
  array(0) {
  }
}