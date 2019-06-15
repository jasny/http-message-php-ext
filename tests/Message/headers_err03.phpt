--TEST--
Message::withoutHeader() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->withoutHeader(['Foo' => 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withoutHeader();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\Message::withoutHeader() expects parameter 1 to be string, array given
HttpMessage\Message::withoutHeader() expects exactly 1 parameter, 0 given