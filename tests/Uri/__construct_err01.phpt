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
--EXPECTF--
%sttpMessage\Uri::__construct()%s string, array given
