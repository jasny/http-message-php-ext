--TEST--
Factory::createStreamFromFile() with non-existing file
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createStreamFromFile(__DIR__ . '/not/exists');
} catch (RuntimeException $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Failed to open '/home/arnold/Projects/php/http-message/tests/Factory/not/exists' stream