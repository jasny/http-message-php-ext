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
--EXPECTF--
%sttpMessage\Factory::createStream()%sstring, array given
