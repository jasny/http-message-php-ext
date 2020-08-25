--TEST--
Factory::createUri()
--FILE--
<?php
$factory = new HttpMessage\Factory();
$uri = $factory->createUri("https://example.com/path?foo=1&bar=2#frag");

var_dump($uri);

?>
--EXPECTF--
object(HttpMessage\Uri)#%d (7) {
  ["scheme":"HttpMessage\Uri":private]=>
  string(5) "https"
  ["userInfo":"HttpMessage\Uri":private]=>
  string(0) ""
  ["host":"HttpMessage\Uri":private]=>
  string(11) "example.com"
  ["port":"HttpMessage\Uri":private]=>
  NULL
  ["path":"HttpMessage\Uri":private]=>
  string(5) "/path"
  ["query":"HttpMessage\Uri":private]=>
  string(11) "foo=1&bar=2"
  ["fragment":"HttpMessage\Uri":private]=>
  string(4) "frag"
}

