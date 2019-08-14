<?php

// Simple smoke test.

new HttpMessage\Uri();
new HttpMessage\Stream();
new HttpMessage\UploadedFile(__FILE__);

new HttpMessage\Request();
new HttpMessage\ServerRequest();
new HttpMessage\Response();

new HttpMessage\Factory();

echo "ok";
