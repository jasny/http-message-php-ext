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
object(HttpMessage\ServerRequest)#1 (12) {
  ["protocolVersion":protected]=>
  string(3) "1.1"
  ["headers":protected]=>
  array(1) {
    ["Host"]=>
    array(1) {
      [0]=>
      string(11) "example.com"
    }
  }
  ["body":protected]=>
  object(HttpMessage\Stream)#2 (1) {
    ["stream":protected]=>
    resource(%d) of type (stream)
  }
  ["requestTarget":protected]=>
  NULL
  ["method":protected]=>
  string(4) "POST"
  ["uri":protected]=>
  object(HttpMessage\Uri)#3 (7) {
    ["scheme":protected]=>
    string(0) ""
    ["userInfo":protected]=>
    string(0) ""
    ["host":protected]=>
    string(11) "example.com"
    ["port":protected]=>
    NULL
    ["path":protected]=>
    string(6) "/foos/"
    ["query":protected]=>
    string(0) ""
    ["fragment":protected]=>
    string(0) ""
  }
  ["serverParams":protected]=>
  array(3) {
    ["REQUEST_METHOD"]=>
    string(4) "POST"
    ["HTTP_HOST"]=>
    string(11) "example.com"
    ["REQUEST_URI"]=>
    string(6) "/foos/"
  }
  ["cookieParams":protected]=>
  array(1) {
    ["session"]=>
    int(999)
  }
  ["queryParams":protected]=>
  array(2) {
    ["id"]=>
    string(2) "42"
    ["persist"]=>
    string(3) "yes"
  }
  ["uploadedFiles":protected]=>
  array(1) {
    ["document"]=>
    object(HttpMessage\UploadedFile)#4 (8) {
      ["stream":protected]=>
      NULL
      ["file":protected]=>
      string(17) "/tmp/uploadedfile"
      ["size":protected]=>
      int(2822)
      ["error":protected]=>
      int(0)
      ["clientFilename":protected]=>
      string(12) "document.pdf"
      ["clientMediaType":protected]=>
      string(15) "application/pdf"
      ["moved":protected]=>
      bool(false)
      ["checkUploaded":protected]=>
      bool(false)
    }
  }
  ["parsedBody":protected]=>
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
  ["attributes":protected]=>
  array(0) {
  }
}