--TEST--
Factory::createRequest() with Uri object
--FILE--
<?php
$uri = new HttpMessage\Uri('https://www.example.com');

$factory = new HttpMessage\Factory();
$request = $factory->createRequest('GET', $uri);

var_dump($request);
var_dump($request->getUri() === $uri);

?>
--EXPECTF--
object(HttpMessage\Request)#%d (6) {
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
  ["requestTarget":"HttpMessage\Request":private]=>
  NULL
  ["method":"HttpMessage\Request":private]=>
  string(3) "GET"
  ["uri":"HttpMessage\Request":private]=>
  object(HttpMessage\Uri)#%d (7) {
    ["scheme":"HttpMessage\Uri":private]=>
    string(5) "https"
    ["userInfo":"HttpMessage\Uri":private]=>
    string(0) ""
    ["host":"HttpMessage\Uri":private]=>
    string(15) "www.example.com"
    ["port":"HttpMessage\Uri":private]=>
    NULL
    ["path":"HttpMessage\Uri":private]=>
    string(0) ""
    ["query":"HttpMessage\Uri":private]=>
    string(0) ""
    ["fragment":"HttpMessage\Uri":private]=>
    string(0) ""
  }
}
bool(true)
