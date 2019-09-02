--TEST--
Factory::createResponse() with status code and reason phrase
--FILE--
<?php
$factory = new HttpMessage\Factory();

$response = $factory->createResponse(400, "Foo Bar");
var_dump($response);

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
  string(7) "Foo Bar"
}