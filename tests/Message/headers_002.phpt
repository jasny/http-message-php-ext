--TEST--
Message headers; added header / without header
--FILE--
<?php

$foofooResponse = (new HttpMessage\Response())
    ->withHeader('Foo', 'bar')
    ->withAddedHeader('Foo', 'baz');
$fooResponse = $foofooResponse->withHeader('Foo', 'box');
$response = $foofooResponse->withoutHeader('Foo');

var_dump($foofooResponse->getHeaders());
var_dump($foofooResponse->hasHeader('Foo'));
var_dump($foofooResponse->getHeader('Foo'));
var_dump($foofooResponse->getHeaderLine('Foo'));

echo "\n";
var_dump($fooResponse->getHeaders());
var_dump($fooResponse->hasHeader('Foo'));
var_dump($fooResponse->getHeader('Foo'));
var_dump($fooResponse->getHeaderLine('Foo'));

echo "\n";
var_dump($response->getHeaders());
var_dump($response->hasHeader('Foo'));
var_dump($response->getHeader('Foo'));
var_dump($response->getHeaderLine('Foo'));

?>
--EXPECT--
array(1) {
  ["Foo"]=>
  array(2) {
    [0]=>
    string(3) "bar"
    [1]=>
    string(3) "baz"
  }
}
bool(true)
array(2) {
  [0]=>
  string(3) "bar"
  [1]=>
  string(3) "baz"
}
string(8) "bar, baz"

array(1) {
  ["Foo"]=>
  array(1) {
    [0]=>
    string(3) "box"
  }
}
bool(true)
array(1) {
  [0]=>
  string(3) "box"
}
string(3) "box"

array(0) {
}
bool(false)
array(0) {
}
string(0) ""
