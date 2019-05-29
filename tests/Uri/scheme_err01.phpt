--TEST--
Uri::withHost() error: invalid argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withScheme(['scheme' => 'http']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
HttpMessage\Uri::withScheme() expects parameter 1 to be string, array given
