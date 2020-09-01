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
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}

try {
    $response->withStatus(200, ['foo', 'bar']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
HttpMessage\Response::withStatus() expects at least 1 parameter, 0 given
HttpMessage\Response::withStatus()%sint, string given
HttpMessage\Response::withStatus()%sstring, array given
