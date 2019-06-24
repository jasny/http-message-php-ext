--TEST--
ServerRequest::withQueryParams()
--FILE--
<?php
$request = (new HttpMessage\ServerRequest)
    ->withQueryParams([
        'foo' => 'bar',
        'color' => 'green',
        'user' => 22,
    ]);

$newRequest = $request->withQueryParams(['user' => 32]);

var_dump($request->getQueryParams());
var_dump($newRequest->getQueryParams());

?>
--EXPECT--
array(3) {
  ["foo"]=>
  string(3) "bar"
  ["color"]=>
  string(5) "green"
  ["user"]=>
  int(22)
}
array(1) {
  ["user"]=>
  int(32)
}