--TEST--
ServerRequest::withAttribute() and ServerRequest::withoutAttribute()
--FILE--
<?php
$request = (new HttpMessage\ServerRequest)
    ->withAttribute('foo', 'bar')
    ->withAttribute('colors', ['red', 'green'])
    ->withAttribute('user', ['id' => 22, 'name' => 'John']);

$newRequest = $request
    ->withoutAttribute('foo')
    ->withAttribute('user', ['id' => 32, 'name' => 'Jane']);

var_dump($request->getAttributes());
var_dump($newRequest->getAttributes());

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
    int(32)
    ["name"]=>
    string(4) "Jane"
  }
}