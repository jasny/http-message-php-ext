--TEST--
Message::withBody() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $resource = fopen('php://memory', 'w+');
    $response->withBody($resource);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withBody();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
%sttpMessage\Message::withBody()%s Psr\Http\Message\StreamInterface, resource given
HttpMessage\Message::withBody() expects exactly 1 parameter, 0 given
