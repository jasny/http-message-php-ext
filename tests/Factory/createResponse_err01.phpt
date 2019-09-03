--TEST--
Factory::createResponse() with invalid arguments
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $factory->createResponse('ok');
} catch (TypeError $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    $factory->createResponse(200, ['foo', 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Argument 1 passed to HttpMessage\Factory::createResponse() must be of the type int, string given
Argument 2 passed to HttpMessage\Factory::createResponse() must be of the type string, array given
