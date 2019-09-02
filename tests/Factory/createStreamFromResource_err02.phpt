--TEST--
Factory::createStreamFromResource() with closed stream
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fclose($resource);

$factory = new HttpMessage\Factory();

try {
    $factory->createStreamFromResource($resource);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Resource is not a stream
