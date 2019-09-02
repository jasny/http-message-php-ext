--TEST--
Factory::createResponse() with status code
--FILE--
<?php
$factory = new HttpMessage\Factory();

$response = $factory->createResponse(400);
var_dump($response);

var_dump($factory->createResponse(100)->getStatusCode());
var_dump($factory->createResponse(100)->getReasonPhrase());
var_dump($factory->createResponse(200)->getReasonPhrase());
var_dump($factory->createResponse(404)->getReasonPhrase());

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
  int(400)
  ["reasonPhrase":"HttpMessage\Response":private]=>
  string(11) "Bad Request"
}
int(100)
string(8) "Continue"
string(2) "OK"
string(9) "Not Found"
