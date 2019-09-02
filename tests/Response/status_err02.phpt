--TEST--
Response::withStatus() with invalid status code
--FILE--
<?php
$response = new HttpMessage\Response();

try {
    $response->withStatus(-1);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECT--
Invalid HTTP status code -1
