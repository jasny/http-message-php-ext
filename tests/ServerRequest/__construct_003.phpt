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
  object(HttpMessage\UploadedFile)#%d (8) {
    ["stream":"HttpMessage\UploadedFile":private]=>
    NULL
    ["file":"HttpMessage\UploadedFile":private]=>
    string(%d) "%suploadedfile01"
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
  ["attachments"]=>
  array(2) {
    [0]=>
    object(HttpMessage\UploadedFile)#%d (8) {
      ["stream":"HttpMessage\UploadedFile":private]=>
      NULL
      ["file":"HttpMessage\UploadedFile":private]=>
      string(%d) "%suploadedfile02"
      ["size":"HttpMessage\UploadedFile":private]=>
      int(3724)
      ["error":"HttpMessage\UploadedFile":private]=>
      int(0)
      ["clientFilename":"HttpMessage\UploadedFile":private]=>
      string(11) "copy-id.pdf"
      ["clientMediaType":"HttpMessage\UploadedFile":private]=>
      string(9) "image/jpg"
      ["moved":"HttpMessage\UploadedFile":private]=>
      bool(false)
      ["checkUploaded":"HttpMessage\UploadedFile":private]=>
      bool(false)
    }
    [1]=>
    object(HttpMessage\UploadedFile)#%d (8) {
      ["stream":"HttpMessage\UploadedFile":private]=>
      NULL
      ["file":"HttpMessage\UploadedFile":private]=>
      string(%d) "%suploadedfile03"
      ["size":"HttpMessage\UploadedFile":private]=>
      int(263)
      ["error":"HttpMessage\UploadedFile":private]=>
      int(0)
      ["clientFilename":"HttpMessage\UploadedFile":private]=>
      string(22) "proof-of-residence.pdf"
      ["clientMediaType":"HttpMessage\UploadedFile":private]=>
      string(15) "application/pdf"
      ["moved":"HttpMessage\UploadedFile":private]=>
      bool(false)
      ["checkUploaded":"HttpMessage\UploadedFile":private]=>
      bool(false)
    }
  }
  ["forms"]=>
  array(3) {
    ["g201"]=>
    object(HttpMessage\UploadedFile)#%d (8) {
      ["stream":"HttpMessage\UploadedFile":private]=>
      NULL
      ["file":"HttpMessage\UploadedFile":private]=>
      string(%d) "%suploadedfile04"
      ["size":"HttpMessage\UploadedFile":private]=>
      int(942)
      ["error":"HttpMessage\UploadedFile":private]=>
      int(0)
      ["clientFilename":"HttpMessage\UploadedFile":private]=>
      NULL
      ["clientMediaType":"HttpMessage\UploadedFile":private]=>
      NULL
      ["moved":"HttpMessage\UploadedFile":private]=>
      bool(false)
      ["checkUploaded":"HttpMessage\UploadedFile":private]=>
      bool(false)
    }
    ["g202"]=>
    array(3) {
      ["a"]=>
      object(HttpMessage\UploadedFile)#%d (8) {
        ["stream":"HttpMessage\UploadedFile":private]=>
        NULL
        ["file":"HttpMessage\UploadedFile":private]=>
        string(%d) "%suploadedfile05"
        ["size":"HttpMessage\UploadedFile":private]=>
        int(2391)
        ["error":"HttpMessage\UploadedFile":private]=>
        int(0)
        ["clientFilename":"HttpMessage\UploadedFile":private]=>
        NULL
        ["clientMediaType":"HttpMessage\UploadedFile":private]=>
        NULL
        ["moved":"HttpMessage\UploadedFile":private]=>
        bool(false)
        ["checkUploaded":"HttpMessage\UploadedFile":private]=>
        bool(false)
      }
      ["b"]=>
      object(HttpMessage\UploadedFile)#%d (8) {
        ["stream":"HttpMessage\UploadedFile":private]=>
        NULL
        ["file":"HttpMessage\UploadedFile":private]=>
        NULL
        ["size":"HttpMessage\UploadedFile":private]=>
        NULL
        ["error":"HttpMessage\UploadedFile":private]=>
        int(4)
        ["clientFilename":"HttpMessage\UploadedFile":private]=>
        NULL
        ["clientMediaType":"HttpMessage\UploadedFile":private]=>
        NULL
        ["moved":"HttpMessage\UploadedFile":private]=>
        bool(false)
        ["checkUploaded":"HttpMessage\UploadedFile":private]=>
        bool(false)
      }
      ["c"]=>
      object(HttpMessage\UploadedFile)#%d (8) {
        ["stream":"HttpMessage\UploadedFile":private]=>
        NULL
        ["file":"HttpMessage\UploadedFile":private]=>
        NULL
        ["size":"HttpMessage\UploadedFile":private]=>
        int(485)
        ["error":"HttpMessage\UploadedFile":private]=>
        int(3)
        ["clientFilename":"HttpMessage\UploadedFile":private]=>
        NULL
        ["clientMediaType":"HttpMessage\UploadedFile":private]=>
        NULL
        ["moved":"HttpMessage\UploadedFile":private]=>
        bool(false)
        ["checkUploaded":"HttpMessage\UploadedFile":private]=>
        bool(false)
      }
    }
    ["g301"]=>
    array(2) {
      [0]=>
      object(HttpMessage\UploadedFile)#%d (8) {
        ["stream":"HttpMessage\UploadedFile":private]=>
        NULL
        ["file":"HttpMessage\UploadedFile":private]=>
        string(%d) "%suploadedfile07"
        ["size":"HttpMessage\UploadedFile":private]=>
        int(4732)
        ["error":"HttpMessage\UploadedFile":private]=>
        int(0)
        ["clientFilename":"HttpMessage\UploadedFile":private]=>
        NULL
        ["clientMediaType":"HttpMessage\UploadedFile":private]=>
        NULL
        ["moved":"HttpMessage\UploadedFile":private]=>
        bool(false)
        ["checkUploaded":"HttpMessage\UploadedFile":private]=>
        bool(false)
      }
      [1]=>
      object(HttpMessage\UploadedFile)#%d (8) {
        ["stream":"HttpMessage\UploadedFile":private]=>
        NULL
        ["file":"HttpMessage\UploadedFile":private]=>
        NULL
        ["size":"HttpMessage\UploadedFile":private]=>
        int(124901432)
        ["error":"HttpMessage\UploadedFile":private]=>
        int(2)
        ["clientFilename":"HttpMessage\UploadedFile":private]=>
        NULL
        ["clientMediaType":"HttpMessage\UploadedFile":private]=>
        NULL
        ["moved":"HttpMessage\UploadedFile":private]=>
        bool(false)
        ["checkUploaded":"HttpMessage\UploadedFile":private]=>
        bool(false)
      }
    }
  }
}