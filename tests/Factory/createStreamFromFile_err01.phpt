--TEST--
Factory::createStreamFromFile() with invalid args
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createStreamFromFile([]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $factory->createStreamFromFile(__FILE__, []);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Argument 1 passed to HttpMessage\Factory::createStreamFromFile() must be of the type string, array given
Argument 2 passed to HttpMessage\Factory::createStreamFromFile() must be of the type string, array given