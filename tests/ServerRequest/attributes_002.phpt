--TEST--
ServerRequest::getAttribute()
--FILE--
<?php
$request = (new HttpMessage\ServerRequest)
    ->withAttribute('foo', 'bar')
    ->withAttribute('colors', ['red', 'green'])
    ->withAttribute('user', ['id' => 22, 'name' => 'John']);

var_dump($request->getAttributes());
var_dump($request->getAttribute('colors', []));
var_dump($request->getAttribute('qux', 42));

?>
--EXPECT--
array(3) {
  ["foo"]=>
  string(3) "bar"
  ["colors"]=>
  array(2) {
    [0]=>
    string(3) "red"
    [1]=>
    string(5) "green"
  }
  ["user"]=>
  array(2) {
    ["id"]=>
    int(22)
    ["name"]=>
    string(4) "John"
  }
}
array(2) {
  [0]=>
  string(3) "red"
  [1]=>
  string(5) "green"
}
int(42)