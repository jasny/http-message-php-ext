--TEST--
Factory::createServerRequest() with invalid arguments
--FILE--
<?php
$factory = new HttpMessage\Factory();

try {
    $request = $factory->createServerRequest();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request = $factory->createServerRequest(42);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request = $factory->createServerRequest("GET", []);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $request = $factory->createServerRequest("GET", "http://www.example.com", "hello");
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\Factory::createServerRequest() expects at least 2 parameters, 0 given
HttpMessage\Factory::createServerRequest() expects at least 2 parameters, 1 given
HttpMessage\Factory::createServerRequest() expects parameter 1 to be a string or object that implements Psr\Http\Message\UriInterface, array given
%sttpMessage\Factory::createServerRequest()%sarray, string given
