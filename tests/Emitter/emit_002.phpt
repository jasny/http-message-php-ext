--TEST--
Emitter::emit() with a response body
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = new HttpMessage\Response();
$response->getBody()->write("Hello World");

$emitter->emit($response);

?>
--EXPECTHEADERS--
--EXPECT--
Hello World
