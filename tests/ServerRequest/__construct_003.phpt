--TEST--
Create a ServerRequest with multiple uploaded files
--FILE--
<?php
$request = new HttpMessage\ServerRequest(
    [],
    [],
    [],
    [],
    [
        'document' => [
            'tmp_name' => sys_get_temp_dir() . '/uploadedfile01',
            'size' => 2822,
            'error' => UPLOAD_ERR_OK,
            'name' => 'document.pdf',
            'type' => 'application/pdf',
        ],
        'attachments' => [
            'tmp_name' => [
                sys_get_temp_dir() . '/uploadedfile02',
                sys_get_temp_dir() . '/uploadedfile03',
            ],
            'size' => [
                3724,
                263,
            ],
            'error' => [
                UPLOAD_ERR_OK,
                UPLOAD_ERR_OK,
            ],
            'name' => [
                'copy-id.pdf',
                'proof-of-residence.pdf',
            ],
            'type' => [
                'image/jpg',
                'application/pdf',
            ],
        ],
        'forms' => [
            'tmp_name' => [
                'g201' => sys_get_temp_dir() . '/uploadedfile04',
                'g202' => [
                    'a' => sys_get_temp_dir() . '/uploadedfile05',
                    'c' => sys_get_temp_dir() . '/uploadedfile06',
                ],
                'g301' => [
                    sys_get_temp_dir() . '/uploadedfile07',
                ],
            ],
            'size' => [
                'g201' => 942,
                'g202' => [
                    'a' => 2391,
                    'c' => 485,
                ],
                'g301' => [
                    4732,
                    124901432,
                ],
            ],
            'error' => [
                'g201' => UPLOAD_ERR_OK,
                'g202' => [
                    'a' => UPLOAD_ERR_OK,
                    'b' => UPLOAD_ERR_NO_FILE,
                    'c' => UPLOAD_ERR_PARTIAL,
                ],
                'g301' => [
                    UPLOAD_ERR_OK,
                    UPLOAD_ERR_FORM_SIZE,
                ],
            ],
            'name' => [
            ],
            'type' => [
            ],
        ],
    ]
);

var_dump($request->getUploadedFiles());

?>
--EXPECTF--
array(3) {
  ["document"]=>
  object(HttpMessage\UploadedFile)#4 (8) {
    ["stream":protected]=>
    NULL
    ["file":protected]=>
    string(%d) "%s/uploadedfile01"
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
  ["attachments"]=>
  array(2) {
    [0]=>
    object(HttpMessage\UploadedFile)#5 (8) {
      ["stream":protected]=>
      NULL
      ["file":protected]=>
      string(%d) "%s/uploadedfile02"
      ["size":protected]=>
      int(3724)
      ["error":protected]=>
      int(0)
      ["clientFilename":protected]=>
      string(11) "copy-id.pdf"
      ["clientMediaType":protected]=>
      string(9) "image/jpg"
      ["moved":protected]=>
      bool(false)
      ["checkUploaded":protected]=>
      bool(false)
    }
    [1]=>
    object(HttpMessage\UploadedFile)#6 (8) {
      ["stream":protected]=>
      NULL
      ["file":protected]=>
      string(%d) "%s/uploadedfile03"
      ["size":protected]=>
      int(263)
      ["error":protected]=>
      int(0)
      ["clientFilename":protected]=>
      string(22) "proof-of-residence.pdf"
      ["clientMediaType":protected]=>
      string(15) "application/pdf"
      ["moved":protected]=>
      bool(false)
      ["checkUploaded":protected]=>
      bool(false)
    }
  }
  ["forms"]=>
  array(3) {
    ["g201"]=>
    object(HttpMessage\UploadedFile)#7 (8) {
      ["stream":protected]=>
      NULL
      ["file":protected]=>
      string(%d) "%s/uploadedfile04"
      ["size":protected]=>
      int(942)
      ["error":protected]=>
      int(0)
      ["clientFilename":protected]=>
      NULL
      ["clientMediaType":protected]=>
      NULL
      ["moved":protected]=>
      bool(false)
      ["checkUploaded":protected]=>
      bool(false)
    }
    ["g202"]=>
    array(3) {
      ["a"]=>
      object(HttpMessage\UploadedFile)#8 (8) {
        ["stream":protected]=>
        NULL
        ["file":protected]=>
        string(%d) "%s/uploadedfile05"
        ["size":protected]=>
        int(2391)
        ["error":protected]=>
        int(0)
        ["clientFilename":protected]=>
        NULL
        ["clientMediaType":protected]=>
        NULL
        ["moved":protected]=>
        bool(false)
        ["checkUploaded":protected]=>
        bool(false)
      }
      ["b"]=>
      object(HttpMessage\UploadedFile)#9 (8) {
        ["stream":protected]=>
        NULL
        ["file":protected]=>
        NULL
        ["size":protected]=>
        NULL
        ["error":protected]=>
        int(4)
        ["clientFilename":protected]=>
        NULL
        ["clientMediaType":protected]=>
        NULL
        ["moved":protected]=>
        bool(false)
        ["checkUploaded":protected]=>
        bool(false)
      }
      ["c"]=>
      object(HttpMessage\UploadedFile)#10 (8) {
        ["stream":protected]=>
        NULL
        ["file":protected]=>
        NULL
        ["size":protected]=>
        int(485)
        ["error":protected]=>
        int(3)
        ["clientFilename":protected]=>
        NULL
        ["clientMediaType":protected]=>
        NULL
        ["moved":protected]=>
        bool(false)
        ["checkUploaded":protected]=>
        bool(false)
      }
    }
    ["g301"]=>
    array(2) {
      [0]=>
      object(HttpMessage\UploadedFile)#11 (8) {
        ["stream":protected]=>
        NULL
        ["file":protected]=>
        string(%d) "%s/uploadedfile07"
        ["size":protected]=>
        int(4732)
        ["error":protected]=>
        int(0)
        ["clientFilename":protected]=>
        NULL
        ["clientMediaType":protected]=>
        NULL
        ["moved":protected]=>
        bool(false)
        ["checkUploaded":protected]=>
        bool(false)
      }
      [1]=>
      object(HttpMessage\UploadedFile)#12 (8) {
        ["stream":protected]=>
        NULL
        ["file":protected]=>
        NULL
        ["size":protected]=>
        int(124901432)
        ["error":protected]=>
        int(2)
        ["clientFilename":protected]=>
        NULL
        ["clientMediaType":protected]=>
        NULL
        ["moved":protected]=>
        bool(false)
        ["checkUploaded":protected]=>
        bool(false)
      }
    }
  }
}