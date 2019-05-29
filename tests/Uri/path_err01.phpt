--TEST--
Uri::withPath() error: invalid argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withPath(['path' => '/foo']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withPath() expects parameter 1 to be string, array given
