--TEST--
Message::withAddedHeader() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->withAddedHeader('Foo', ['a', 'b']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withAddedHeader(['Foo' => 'bar']);
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withAddedHeader(['Foo' => 'bar'], null);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\Message::withAddedHeader()%sstring, array given
HttpMessage\Message::withAddedHeader() expects exactly 2 %s, 1 given
HttpMessage\Message::withAddedHeader()%sstring, array given
