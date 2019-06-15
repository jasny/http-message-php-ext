--TEST--
Create a Response
--FILE--
<?php
$response = new HttpMessage\Response();

var_dump($response);
var_dump($response->getBody()->getMetadata('uri'));
?>
--EXPECTF--
object(HttpMessage\Response)#1 (5) {
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
  ["statusCode":protected]=>
  int(0)
  ["reasonPhrase":protected]=>
  string(0) ""
}
string(10) "php://temp"
