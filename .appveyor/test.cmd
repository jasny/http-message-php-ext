@echo off

setlocal enableextensions enabledelayedexpansion

    cd C:\projects\php-sdk\phpdev\vc14\x64\php-src
	
	pushd ext\base58
    echo [PHP] > php.ini
    echo extension_dir = "ext" >> php.ini
    echo extension=php_http_message.dll >> php.ini
	popd

	set TEST_PHP_EXECUTABLE=C:\projects\php-sdk\phpdev\vc14\x64\php-src\x64\Release_TS\php.exe
	%TEST_PHP_EXECUTABLE% run-tests.php ext\http_message -q --show-diff

endlocal
