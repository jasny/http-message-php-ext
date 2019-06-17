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
var_dump($arrRequest->getParsedBody());

$objRequest = $request->withParsedBody((object)[
    'color' => 'red',
    'user' => 42,
]);
var_dump($objRequest->getParsedBody());

$nullRequest = $objRequest->withParsedBody(null);
var_dump($nullRequest->getParsedBody());

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
object(stdClass)#5 (2) {
  ["color"]=>
  string(3) "red"
  ["user"]=>
  int(42)
}
NULL