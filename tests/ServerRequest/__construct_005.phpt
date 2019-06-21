--TEST--
Create a ServerRequest with server params for uri
--FILE--
<?php
// HTTP
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 80,
    ]
);
echo $request->getUri(), "\n";

// HTTP (explicit)
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTPS' => 'off',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 80,
    ]
);
echo $request->getUri(), "\n";

// HTTPS
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTPS' => 'on',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 443,
    ]
);
echo $request->getUri(), "\n";

// Custom HTTP port
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTPS' => 'off',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 8080,
    ]
);
echo $request->getUri(), "\n";

// HTTPS on port 80
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTPS' => 'on',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 80,
    ]
);
echo $request->getUri(), "\n";

// No protocol, but default HTTP port
$request = new HttpMessage\ServerRequest(
    [
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 80,
    ]
);
echo $request->getUri(), "\n";

// No protocol, but default HTTPS port
$request = new HttpMessage\ServerRequest(
    [
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'SERVER_PORT' => 80,
    ]
);
echo $request->getUri(), "\n";

// No protocol, no port
$request = new HttpMessage\ServerRequest(
    [
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
    ]
);
echo $request->getUri(), "\n";

// User
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'PHP_AUTH_USER' => 'john'
    ]
);
echo $request->getUri(), "\n";

// User + pass
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'PHP_AUTH_USER' => 'john',
        'PHP_AUTH_PASS' => 'secret',
    ]
);
echo $request->getUri(), "\n";

// Only pass (ignored)
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'PHP_AUTH_PASS' => 'secret',
    ]
);
echo $request->getUri(), "\n";

// Query string overwrite
$request = new HttpMessage\ServerRequest(
    [
        'SERVER_PROTOCOL' => 'HTTP/1.1',
        'HTTP_HOST' => 'example.com',
        'REQUEST_URI' => '/foos/?bar=1',
        'QUERY_STRING' => 'bar=99',
    ]
);
echo $request->getUri(), "\n";


?>
--EXPECT--
http://example.com/foos/?bar=1
http://example.com/foos/?bar=1
https://example.com/foos/?bar=1
http://example.com:8080/foos/?bar=1
https://example.com:80/foos/?bar=1
http://example.com/foos/?bar=1
http://example.com/foos/?bar=1
//example.com/foos/?bar=1
http://john@example.com/foos/?bar=1
http://john:secret@example.com/foos/?bar=1
http://example.com/foos/?bar=1
http://example.com/foos/?bar=99