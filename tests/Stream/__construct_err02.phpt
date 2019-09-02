--TEST--
Create Stream with invalid argument
--FILE--
<?php
try {
    new HttpMessage\Stream(['abc']);
} catch (TypeError $e) {
    echo $e->getMessage();
}
?>
--EXPECT--
Expected parameter 1 to be a string or resource, array given
