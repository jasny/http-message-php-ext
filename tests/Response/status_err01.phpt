--TEST--
Response::withStatus() with invalid arguments
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->withStatus();
} catch (ArgumentCountError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withStatus('ok');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    $response->withStatus(200, ['foo', 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
HttpMessage\Response::withStatus() expects at least 1 parameter, 0 given
HttpMessage\Response::withStatus() expects parameter 1 to be integer, string given
HttpMessage\Response::withStatus() expects parameter 2 to be string, array given