--TEST--
Factory::createServerRequest()
--FILE--
<?php
$factory = new HttpMessage\Factory();
$request = $factory->createServerRequest('GET', 'https://www.example.com');

var_dump($request);

?>
--EXPECTF--
object(HttpMessage\ServerRequest)#%d (14) {
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
  ["serverParams":"HttpMessage\ServerRequest":private]=>
  array(0) {
  }
  ["cookieParams":"HttpMessage\ServerRequest":private]=>
  array(0) {
  }
  ["queryParams":"HttpMessage\ServerRequest":private]=>
  array(0) {
  }
  ["uploadedFiles":"HttpMessage\ServerRequest":private]=>
  array(0) {
  }
  ["parsedBody":"HttpMessage\ServerRequest":private]=>
  NULL
  ["attributes":"HttpMessage\ServerRequest":private]=>
  array(0) {
  }
  ["method"]=>
  string(3) "GET"
  ["uri"]=>
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