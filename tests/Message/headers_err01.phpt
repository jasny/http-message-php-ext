--TEST--
Message::withHeader() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->withHeader('Foo', ['a', 'b']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withHeader(['Foo' => 'bar']);
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withHeader(['Foo' => 'bar'], null);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\Message::withHeader()%sstring, array given
HttpMessage\Message::withHeader() expects exactly 2 parameters, 1 given
HttpMessage\Message::withHeader()%sstring, array given
