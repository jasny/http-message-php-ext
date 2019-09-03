--TEST--
Create Stream with closed stream
--FILE--
<?php
$resource = fopen('php://memory', 'w+');
fclose($resource);

try {
    new HttpMessage\Stream($resource);
} catch (InvalidArgumentException $e) {
    echo $e->getMessage();
}
?>
--EXPECT--
Resource is not a stream
