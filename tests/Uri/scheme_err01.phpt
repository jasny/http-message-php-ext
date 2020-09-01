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
--EXPECTF--
HttpMessage\Uri::withScheme()%s string, array given
