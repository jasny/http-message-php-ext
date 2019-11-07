--TEST--
Emitter::emit() with a response with headers
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = (new HttpMessage\Response())
    ->withHeader("Content-Length", 14)
    ->withHeader("Foo", "bar");
$response->getBody()->write("Page not found");

$emitter->emit($response);

?>
--EXPECTHEADERS--
Content-Length: 14
Foo: bar
--EXPECT--
Page not found
