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
  string(0) ""
  ["uri":"HttpMessage\Request":private]=>
  object(HttpMessage\Uri)#%d (7) {
    ["scheme":"HttpMessage\Uri":private]=>
    string(0) ""
    ["userInfo":"HttpMessage\Uri":private]=>
    string(0) ""
    ["host":"HttpMessage\Uri":private]=>
    string(0) ""
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
string(1) "/"
string(0) ""
string(10) "php://temp"