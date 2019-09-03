--TEST--
Factory::createResponse() with invalid status code
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createResponse(-1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Invalid HTTP status code -1