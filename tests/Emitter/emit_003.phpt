--TEST--
Emitter::emit() with a response status
--INI--
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = (new HttpMessage\Response())
    ->withStatus(200, "Ok");
$response->getBody()->write("Hello World");

$emitter->emit($response);
?>
--EXPECTHEADERS--
HTTP/1.1 200 OK
--EXPECT--
Hello World
