--TEST--
Factory::createStreamFromResource() with invalid argument
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createStreamFromResource([]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Expected parameter 1 to be a string or resource, array given