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
--EXPECTF--
HttpMessage\Uri::withQuery()%s string, array given
