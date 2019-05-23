--TEST--
Uri::withScheme()
--FILE--
<?php
$blank = new HttpMessage\Uri();
$https = $blank->withScheme("https");
$mailto = $https->withScheme("mailto");

var_dump($blank->getScheme());
var_dump($https->getScheme());
var_dump($mailto->getScheme());
?>
--EXPECT--
string(0) ""
string(5) "https"
string(6) "mailto"
