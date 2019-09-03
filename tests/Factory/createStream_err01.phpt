--TEST--
Factory::createStream() with invalid argument
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createStream([]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Argument 1 passed to HttpMessage\Factory::createStream() must be of the type string, array given