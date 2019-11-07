--TEST--
Emitter::emit() with a response with response status
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = (new HttpMessage\Response())
    ->withStatus(404)
    ->withHeader("Foo", "Bar");

$emitter->emit($response);

?>
--EXPECTHEADERS--
HTTP/1.1 404 Not Found
Foo: Bar
--EXPECT--
