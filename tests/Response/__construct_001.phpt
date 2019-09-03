--TEST--
Create a Response
--FILE--
<?php
$response = new HttpMessage\Response();

var_dump($response);
var_dump($response->getBody()->getMetadata('uri'));
?>
--EXPECTF--
object(HttpMessage\Response)#%d (5) {
  ["protocolVersion":"HttpMessage\Message":private]=>
  string(3) "1.1"
  ["headers":"HttpMessage\Message":private]=>
  array(0) {
  }
  ["body":"HttpMessage\Message":private]=>
  object(HttpMessage\Stream)#%d (1) {
    ["stream":"HttpMessage\Stream":private]=>
    resource(%d) of type (stream)
  }
  ["statusCode":"HttpMessage\Response":private]=>
  int(0)
  ["reasonPhrase":"HttpMessage\Response":private]=>
  string(0) ""
}
string(10) "php://temp"