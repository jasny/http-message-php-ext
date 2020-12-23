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
--EXPECTF--
HttpMessage\Message::getHeaderLine()%sstring, array given
HttpMessage\Message::getHeaderLine() expects exactly 1 %s, 0 given
