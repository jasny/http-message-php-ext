--TEST--
Message::withProtocolVersion() with invalid argument
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->withProtocolVersion();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withProtocolVersion(['foo', 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Message::withProtocolVersion() expects exactly 1 parameter, 0 given
HttpMessage\Message::withProtocolVersion()%sstring, array given
