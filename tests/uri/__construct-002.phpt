--TEST--
Create Uri with a basic url
--FILE--
<?php
var_dump(new HttpMessage\Uri('https://www.example.com'));
?>
--EXPECT--
class HttpMessage\Uri#1 (7) {
  protected $scheme =>
  string(5) "https"
  protected $userInfo =>
  string(0) ""
  protected $host =>
  string(15) "www.example.com"
  protected $port =>
  NULL
  protected $path =>
  string(1) "/"
  protected $query =>
  string(0) ""
  protected $fragment =>
  string(0) ""
}
