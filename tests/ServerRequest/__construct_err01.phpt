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
--EXPECT--
Argument 1 passed to HttpMessage\ServerRequest::__construct() must be of the type array, string given
Argument 2 passed to HttpMessage\ServerRequest::__construct() must be of the type array, string given
Argument 3 passed to HttpMessage\ServerRequest::__construct() must be of the type array, string given
Argument 3 passed to HttpMessage\ServerRequest::__construct() must be of the type array, string given
Argument 4 passed to HttpMessage\ServerRequest::__construct() must be of the type array, string given
Argument 5 passed to HttpMessage\ServerRequest::__construct() must be of the type array, string given
