--TEST--
Create Uri error: invalid argument
--FILE--
<?php
try {
    new HttpMessage\Uri(['scheme' => 'https']);
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
Argument 1 passed to HttpMessage\Uri::__construct() must be of the type string, array given
