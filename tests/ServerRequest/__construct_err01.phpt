--TEST--
Create ServerRequest with invalid arguments
--FILE--
<?php

try {
    new HttpMessage\ServerRequest('');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\ServerRequest([], '');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\ServerRequest([], [], '');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\ServerRequest([], [], '');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\ServerRequest([], [], [], '');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

try {
    new HttpMessage\ServerRequest([], [], [], [], '');
} catch (TypeError $e) {
    echo $e->getMessage(), "\n";
}

?>
--EXPECTF--
%sttpMessage\ServerRequest::__construct()%sarray, string given
%sttpMessage\ServerRequest::__construct()%sarray, string given
%sttpMessage\ServerRequest::__construct()%sarray, string given
%sttpMessage\ServerRequest::__construct()%sarray, string given
%sttpMessage\ServerRequest::__construct()%sarray, string given
%sttpMessage\ServerRequest::__construct()%sarray, string given
