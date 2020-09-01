--TEST--
Factory::createStreamFromFile() with mode
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $stream = $factory->createStreamFromFile([]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $stream = $factory->createStreamFromFile(__FILE__, []);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
%sttpMessage\Factory::createStreamFromFile()%sstring, array given
%sttpMessage\Factory::createStreamFromFile()%sstring, array given
