--TEST--
Uri::withFragment() error: invalid argument
--FILE--
<?php
try {
    (new HttpMessage\Uri)->withFragment(['fragment' => 'foo']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECTF--
HttpMessage\Uri::withFragment()%s string, array given
