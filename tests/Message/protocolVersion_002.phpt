--TEST--
Message::withProtocolVersion() with float
--FILE--
<?php
$response = (new HttpMessage\Response())
    ->withProtocolVersion(2.0);

var_dump($response->getProtocolVersion());
?>
--EXPECT--
string(1) "2"