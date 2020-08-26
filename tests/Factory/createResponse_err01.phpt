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
--EXPECTF--
%sttpMessage\Factory::createResponse()%sint, string given
%sttpMessage\Factory::createResponse()%sstring, array given
