--TEST--
Emitter::emit() with a response body
--INI--
default_mimetype=text/plain
default_charset=UTF-8
--SKIPIF--
<?php if (!extension_loaded("xdebug")) die("Skipped: xdebug extension required."); ?>
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = new HttpMessage\Response();
$response->getBody()->write("Hello World");

$emitter->emit($response);
echo "\n---\n";

var_dump(http_response_code());
var_dump(xdebug_get_headers());

?>
--EXPECT--
Hello World
---
bool(false)
array(1) {
  [0]=>
  string(39) "Content-type: text/plain; charset=UTF-8"
}