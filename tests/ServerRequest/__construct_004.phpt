--TEST--
Create a ServerRequest with server params
--FILE--
<?php
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'REQUEST_METHOD' => 'POST',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'CONTENT_TYPE' => 'application/x-www-form-urlencoded',
        'CONTENT_LENGTH' => 1324,
        'HTTP_ACCEPT' => 'text/html',
        'HTTP_ACCEPT_CHARSET' => 'utf-8',
        'HTTP_ACCEPT_ENCODING' => 'zip, deflate',
        'PHP_AUTH_USER' => 'john',
        'PHP_AUTH_PASS' => 'secret',
        'SERVER_PORT' => 80,
    ]
);

var_dump($request->getMethod());
var_dump($request->getUri());
var_dump($request->getHeaders());

?>
--EXPECTF--
string(0) ""
object(HttpMessage\Uri)#%d (7) {
  ["scheme":"HttpMessage\Uri":private]=>
  string(4) "http"
  ["userInfo":"HttpMessage\Uri":private]=>
  string(11) "john:secret"
  ["host":"HttpMessage\Uri":private]=>
  string(11) "example.com"
  ["port":"HttpMessage\Uri":private]=>
  NULL
  ["path":"HttpMessage\Uri":private]=>
  string(6) "/foos/"
  ["query":"HttpMessage\Uri":private]=>
  string(5) "bar=1"
  ["fragment":"HttpMessage\Uri":private]=>
  string(0) ""
}
array(6) {
  ["Host"]=>
  array(1) {
    [0]=>
    string(11) "example.com"
  }
  ["Accept"]=>
  array(1) {
    [0]=>
    string(9) "text/html"
  }
  ["Accept-Charset"]=>
  array(1) {
    [0]=>
    string(5) "utf-8"
  }
  ["Accept-Encoding"]=>
  array(1) {
    [0]=>
    string(12) "zip, deflate"
  }
  ["Content-Type"]=>
  array(1) {
    [0]=>
    string(33) "application/x-www-form-urlencoded"
  }
  ["Content-Length"]=>
  array(1) {
    [0]=>
    int(1324)
  }
}