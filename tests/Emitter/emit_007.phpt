--TEST--
Emitter::emit() with a response with response status and custom reason
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = (new HttpMessage\Response())
    ->withProtocolVersion("1.0")
    ->withStatus(404, "Please go away")
    ->withHeader("Foo", "Bar");

$emitter->emit($response);

?>
--EXPECTHEADERS--
HTTP/1.0 404 Please go away
Foo: Bar
--EXPECT--
