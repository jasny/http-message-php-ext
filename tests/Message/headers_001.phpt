--TEST--
Message headers
--FILE--
<?php
$response = new HttpMessage\Response();

var_dump($response->getHeaders());
var_dump($response->hasHeader('Foo'));
var_dump($response->getHeader('Foo'));
var_dump($response->getHeaderLine('Foo'));

echo "\n";
$fooResponse = $response->withHeader('Foo', 'bar');
var_dump($fooResponse->getHeaders());
var_dump($fooResponse->hasHeader('Foo'));
var_dump($fooResponse->getHeader('Foo'));
var_dump($fooResponse->getHeaderLine('Foo'));

echo "\n";
$moreResponse = $fooResponse->withHeader('More', 'red');
var_dump($moreResponse->getHeaders());
var_dump($fooResponse->hasHeader('More'));
var_dump($fooResponse->getHeader('More'));
var_dump($fooResponse->getHeaderLine('More'));
var_dump($fooResponse->getHeaderLine('Foo'));

?>
--EXPECT--
array(0) {
}
bool(false)
array(0) {
}
string(0) ""

array(1) {
  ["Foo"]=>
  array(1) {
    [0]=>
    string(3) "bar"
  }
}
bool(true)
array(1) {
  [0]=>
  string(3) "bar"
}
string(3) "bar"

array(2) {
  ["Foo"]=>
  array(1) {
    [0]=>
    string(3) "bar"
  }
  ["More"]=>
  array(1) {
    [0]=>
    string(3) "red"
  }
}
bool(true)
array(1) {
  [0]=>
  string(3) "red"
}
string(3) "red"
string(3) "bar"