--TEST--
Message::getHeaderLine() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->getHeaderLine(['Foo' => 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->getHeaderLine();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\Message::getHeaderLine() expects parameter 1 to be string, array given
HttpMessage\Message::getHeaderLine() expects exactly 1 parameter, 0 given