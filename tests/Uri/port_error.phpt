--TEST--
Uri::withPort() errors
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withPort(['port' => 80]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    (new HttpMessage\Uri)->withPort();
} catch (Error $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withPort() expects parameter 1 to be integer, array given
HttpMessage\Uri::withPort() expects exactly 1 parameter, 0 given
