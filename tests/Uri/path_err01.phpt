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
--EXPECTF--
HttpMessage\Uri::withPath()%s string, array given
