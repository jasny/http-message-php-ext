--TEST--
Uri::withHost() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withHost(['host' => 'www.example.com']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withHost();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withHost()%s, array given
HttpMessage\Uri::withHost() expects exactly 1 %s, 0 given
