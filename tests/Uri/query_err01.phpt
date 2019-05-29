--TEST--
Uri::withQuery() error: invalid argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withQuery(['foo' => 1]);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withQuery() expects parameter 1 to be string, array given
