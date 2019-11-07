--TEST--
Message header with numeric value
--FILE--
<?php

$response = (new HttpMessage\Response())
    ->withHeader('Content-Length', 100);

var_dump($response->getHeaders());

?>
--EXPECT--
array(1) {
  ["Content-Length"]=>
  array(1) {
    [0]=>
    string(3) "100"
  }
}