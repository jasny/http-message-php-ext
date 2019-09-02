--TEST--
Factory::createServerRequest() with server params
--FILE--
<?php
// Params do not match method and uri.
$params = [
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
];

$factory = new HttpMessage\Factory();
$request = $factory->createServerRequest('QUERY', 'https://www.example.com/foo/bar/22?q=1', $params);

var_dump($request);

?>
--EXPECTF--
object(HttpMessage\ServerRequest)#%d (14) {
  ["protocolVersion":"HttpMessage\Message":private]=>
  string(3) "1.1"
  ["headers":"HttpMessage\Message":private]=>
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
  ["serverParams":"HttpMessage\ServerRequest":private]=>
  array(12) {
    ["SERVER_PROTOCOL"]=>
    string(8) "HTTP/1.1"
    ["REQUEST_METHOD"]=>
    string(4) "POST"
    ["HTTP_HOST"]=>
    string(11) "example.com"
    ["REQUEST_URI"]=>
    string(12) "/foos/?bar=1"
    ["CONTENT_TYPE"]=>
    string(33) "application/x-www-form-urlencoded"
    ["CONTENT_LENGTH"]=>
    int(1324)
    ["HTTP_ACCEPT"]=>
    string(9) "text/html"
    ["HTTP_ACCEPT_CHARSET"]=>
    string(5) "utf-8"
    ["HTTP_ACCEPT_ENCODING"]=>
    string(12) "zip, deflate"
    ["PHP_AUTH_USER"]=>
    string(4) "john"
    ["PHP_AUTH_PASS"]=>
    string(6) "secret"
    ["SERVER_PORT"]=>
    int(80)
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
  string(5) "QUERY"
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
    string(11) "/foo/bar/22"
    ["query":"HttpMessage\Uri":private]=>
    string(3) "q=1"
    ["fragment":"HttpMessage\Uri":private]=>
    string(0) ""
  }
}