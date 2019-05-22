--TEST--
Create Uri with a full url
--FILE--
<?php
var_dump(new HttpMessage\Uri('http://acme@www.example.com:8000/foo?answer=42#question'));
?>
--EXPECT--
class HttpMessage\Uri#1 (7) {
  protected $scheme =>
  string(4) "http"
  protected $userInfo =>
  string(4) "acme"
  protected $host =>
  string(15) "www.example.com"
  protected $port =>
  int(8000)
  protected $path =>
  string(4) "/foo"
  protected $query =>
  string(9) "answer=42"
  protected $fragment =>
  string(8) "question"
}
