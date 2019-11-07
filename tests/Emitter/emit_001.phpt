--TEST--
Emitter::emit() with an empty response
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = new HttpMessage\Response();

$emitter->emit($response);

?>
--EXPECTHEADERS--
--EXPECT--
