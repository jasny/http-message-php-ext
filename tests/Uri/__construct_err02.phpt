--TEST--
Create Uri error: Invalid uri
--FILE--
<?php
try {
    new HttpMessage\Uri('https://');
} catch (UnexpectedValueException $e) {
    echo $e->getMessage(), "\n";
}
?>
--EXPECT--
Invalid uri
