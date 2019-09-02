--TEST--
Factory::createStreamFromResource()
--FILE--
<?php
$resource = fopen('php://memory', 'w+');

$factory = new HttpMessage\Factory();
$stream = $factory->createStreamFromResource($resource);

var_dump($stream);
var_dump($stream->detach() === $resource);
?>
--EXPECTF--
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}
bool(true)