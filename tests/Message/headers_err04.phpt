--TEST--
Message::getHeader() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->getHeader(['Foo' => 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->getHeader();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\Message::getHeader()%sstring, array given
HttpMessage\Message::getHeader() expects exactly 1 parameter, 0 given
