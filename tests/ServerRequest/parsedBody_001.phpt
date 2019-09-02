--TEST--
ServerRequest::withParsedBody()
--FILE--
<?php
$request = new HttpMessage\ServerRequest();

$arrRequest = $request->withParsedBody([
    'foo' => 'bar',
    'color' => 'green',
    'user' => 22,
]);

$objRequest = $request->withParsedBody((object)[
    'color' => 'red',
    'user' => 42,
]);

$nullRequest = $objRequest->withParsedBody(null);

var_dump($arrRequest->getParsedBody());
var_dump($objRequest->getParsedBody());
var_dump($nullRequest->getParsedBody());

?>
--EXPECTF--
array(3) {
  ["foo"]=>
  string(3) "bar"
  ["color"]=>
  string(5) "green"
  ["user"]=>
  int(22)
}
object(stdClass)#%d (2) {
  ["color"]=>
  string(3) "red"
  ["user"]=>
  int(42)
}
NULL