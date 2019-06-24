--TEST--
Uri::withPort() error: invalid argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withPort(['port' => 80]);
} catch (TypeError $e) {
    echo strtr($e->getMessage(), ['integer' => 'int']), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withPort() expects parameter 1 to be int, array given
