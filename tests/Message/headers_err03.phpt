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
--EXPECTF--
HttpMessage\Message::withoutHeader()%sstring, array given
HttpMessage\Message::withoutHeader() expects exactly 1 %s, 0 given
