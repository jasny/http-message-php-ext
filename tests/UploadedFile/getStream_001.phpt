--TEST--
UploadedFile::getStream()
--FILE--
<?php
$path = sys_get_temp_dir() . '/uploadedfile';
file_put_contents($path, "foo");

$upload = new HttpMessage\UploadedFile($path, 99, UPLOAD_ERR_OK, 'myfile.txt', 'text/plain');

$stream = $upload->getStream();

var_dump($stream);
var_dump($stream->getMetadata('uri'));
var_dump((string)$stream);
var_dump($stream == $upload->getStream());

?>
--CLEAN--
<?php
unlink(sys_get_temp_dir() . '/uploadedfile');
?>
--EXPECTF--
object(HttpMessage\Stream)#%d (1) {
  ["stream":"HttpMessage\Stream":private]=>
  resource(%d) of type (stream)
}
string(%d) "%s/uploadedfile"
string(3) "foo"
bool(true)