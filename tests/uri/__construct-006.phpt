--TEST--
Create Uri with a domain name
--FILE--
<?php
var_dump(new HttpMessage\Uri('www.example.com'));
?>
--EXPECT--
class HttpMessage\Uri#1 (7) {
  protected $scheme =>
  string(0) ""
  protected $userInfo =>
  string(0) ""
  protected $host =>
  string(0) "www.example.com"
  protected $port =>
  NULL
  protected $path =>
  string(0) ""
  protected $query =>
  string(0) ""
  protected $fragment =>
  string(0) ""
}
