--TEST--
Create Stream with non-existing file
--FILE--
<?php
try {
    new HttpMessage\Stream(__DIR__ . '/not/exists', 'r');
} catch (RuntimeException $e) {
    echo strtr($e->getMessage(), [DIRECTORY_SEPARATOR => '/']);
}
?>
--EXPECTF--
Failed to open '%s/Stream/not/exists' stream
