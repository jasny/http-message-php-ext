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
--EXPECT--
HttpMessage\Uri::withFragment() expects parameter 1 to be string, array given
