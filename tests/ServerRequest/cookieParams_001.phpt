--TEST--
ServerRequest::withCookieParams()
--FILE--
<?php
$request = (new HttpMessage\ServerRequest)
    ->withCookieParams([
        'foo' => 'bar',
        'color' => 'green',
        'user' => 22,
    ]);

$newRequest = $request->withCookieParams(['user' => 32]);

var_dump($request->getCookieParams());
var_dump($newRequest->getCookieParams());

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