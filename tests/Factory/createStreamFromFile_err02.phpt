--TEST--
Factory::createStreamFromFile() with non-existing file
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createStreamFromFile(__DIR__ . '/not/exists');
} catch (RuntimeException $e) {
    echo strtr($e->getMessage(), [DIRECTORY_SEPARATOR => '/']), "\n";
}

?>
--EXPECTF--
Failed to open '%s/Factory/not/exists' stream