--TEST--
Create a ServerRequest with params
--FILE--
<?php
$request = new HttpMessage\ServerRequest(
    [
        'REQUEST_METHOD' => 'POST',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/',
    ],
    ['session' => 999],
    ['id' => '42', 'persist' => 'yes'],
    ['id' => 42, 'date' => '2019-01-01', 'categories' => [22, 44]],
    [
        'document' => [
            'tmp_name' => sys_get_temp_dir() . '/uploadedfile',
            'size' => 2822,
            'error' => UPLOAD_ERR_OK,
            'name' => 'document.pdf',
            'type' => 'application/pdf',
        ],
    ]
);

var_dump($request);

?>
--EXPECTF--
object(HttpMessage\ServerRequest)#%d (13) {
  ["protocolVersion":"HttpMessage\Message":private]=>
  string(3) "1.1"
  ["headers":"HttpMessage\Message":private]=>
  array(1) {
    ["Host"]=>
    array(1) {
      [0]=>
      string(11) "example.com"
    }
  }
  ["body":"HttpMessage\Message":private]=>
  object(HttpMessage\Stream)#%d (1) {
    ["stream":"HttpMessage\Stream":private]=>
    resource(%d) of type (stream)
  }
  ["requestTarget":"HttpMessage\Request":private]=>
  NULL
  ["method":"HttpMessage\Request":private]=>
  string(0) ""
  ["uri":"HttpMessage\Request":private]=>
  object(HttpMessage\Uri)#%d (7) {
    ["scheme":"HttpMessage\Uri":private]=>
    string(0) ""
    ["userInfo":"HttpMessage\Uri":private]=>
    string(0) ""
    ["host":"HttpMessage\Uri":private]=>
    string(11) "example.com"
    ["port":"HttpMessage\Uri":private]=>
    NULL
    ["path":"HttpMessage\Uri":private]=>
    string(6) "/foos/"
    ["query":"HttpMessage\Uri":private]=>
    string(0) ""
    ["fragment":"HttpMessage\Uri":private]=>
    string(0) ""
  }
  ["serverParams":"HttpMessage\ServerRequest":private]=>
  array(3) {
    ["REQUEST_METHOD"]=>
    string(4) "POST"
    ["HTTP_HOST"]=>
    string(11) "example.com"
    ["REQUEST_URI"]=>
    string(6) "/foos/"
  }
  ["cookieParams":"HttpMessage\ServerRequest":private]=>
  array(1) {
    ["session"]=>
    int(999)
  }
  ["queryParams":"HttpMessage\ServerRequest":private]=>
  array(2) {
    ["id"]=>
    string(2) "42"
    ["persist"]=>
    string(3) "yes"
  }
  ["uploadedFiles":"HttpMessage\ServerRequest":private]=>
  array(1) {
    ["document"]=>
    object(HttpMessage\UploadedFile)#%d (8) {
      ["stream":"HttpMessage\UploadedFile":private]=>
      NULL
      ["file":"HttpMessage\UploadedFile":private]=>
      string(%d) "%suploadedfile"
      ["size":"HttpMessage\UploadedFile":private]=>
      int(2822)
      ["error":"HttpMessage\UploadedFile":private]=>
      int(0)
      ["clientFilename":"HttpMessage\UploadedFile":private]=>
      string(12) "document.pdf"
      ["clientMediaType":"HttpMessage\UploadedFile":private]=>
      string(15) "application/pdf"
      ["moved":"HttpMessage\UploadedFile":private]=>
      bool(false)
      ["checkUploaded":"HttpMessage\UploadedFile":private]=>
      bool(false)
    }
  }
  ["parsedBody":"HttpMessage\ServerRequest":private]=>
  array(3) {
    ["id"]=>
    int(42)
    ["date"]=>
    string(10) "2019-01-01"
    ["categories"]=>
    array(2) {
      [0]=>
      int(22)
      [1]=>
      int(44)
    }
  }
  ["attributes":"HttpMessage\ServerRequest":private]=>
  array(0) {
  }
  ["method"]=>
  string(4) "POST"
}