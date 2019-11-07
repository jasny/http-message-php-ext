--TEST--
Emitter::emit() with a response with headers
--INI--
default_mimetype=text/plain
default_charset=UTF-8
--SKIPIF--
<?php if (!extension_loaded("xdebug")) die("Skipped: xdebug extension required."); ?>
--FILE--
<?php
$emitter = new HttpMessage\Emitter();
$response = (new HttpMessage\Response())
    ->withStatus(404, "Not Found")
    ->withHeader("Content-Type", "text/html")
    ->withHeader("Content-Length", 14)
    ->withHeader("Foo", "bar");
$response->getBody()->write("Page not found");

$emitter->emit($response);
echo "\n---\n";

var_dump(http_response_code());
var_dump(xdebug_get_headers());

?>
--EXPECT--
Page not found
---
int(404)
array(3) {
  [0]=>
  string(37) "Content-type: text/html;charset=UTF-8"
  [1]=>
  string(18) "Content-Length: 14"
  [2]=>
  string(8) "Foo: bar"
}
