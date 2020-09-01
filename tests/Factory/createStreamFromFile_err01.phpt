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
--EXPECTF--
%sttpMessage\Factory::createStreamFromFile()%sstring, array given
%sttpMessage\Factory::createStreamFromFile()%sstring, array given
