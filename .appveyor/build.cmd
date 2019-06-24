@echo off

setlocal enableextensions enabledelayedexpansion

	REM set up PHP
	mkdir C:\projects\php-sdk >NUL 2>NUL
	cd C:\projects\php-sdk
    wget %PHP_SDK_BINARY_TOOLS_URL%/%PHP_SDK_BINARY_TOOLS_PACKAGE% --no-check-certificate -q -O %PHP_SDK_BINARY_TOOLS_PACKAGE%
    7z x -y %PHP_SDK_BINARY_TOOLS_PACKAGE%
	cmd /c bin\phpsdk_buildtree.bat phpdev
	pushd phpdev
	ren vc9 vc14
	pushd vc14\x64
    git clone https://git.php.net/repository/php-src.git
	cd php-src
	git checkout PHP-%PHP_REL%
	cd ..
	wget %PHP_DEPS_URL%/%PHP_DEPS_PACKAGE% --no-check-certificate -q -O %PHP_DEPS_PACKAGE%
	7z x -y %PHP_DEPS_PACKAGE%
	popd
	popd

  REM download and install the psr extension from PECL
	mkdir c:\projects\php-sdk\phpdev\vc14\x64\php-src\ext\psr
	wget %PECL_PSR_URL% --no-check-certificate -q -O psr.tgz
  7z x -y psr.tgz -oc:\projects\php-sdk\phpdev\vc14\x64\php-src\ext\psr

	REM copy the extension into the PHP tree
	mkdir c:\projects\php-sdk\phpdev\vc14\x64\php-src\ext\http_message
	xcopy c:\projects\http_message-php-ext\*.* c:\projects\php-sdk\phpdev\vc14\x64\php-src\ext\http_message /s/e/v
	
	REM The bison utility is needed for the PHP build, so add MSYS to the path.
	REM Note: Add to the end to ensure MSVC tools are found firts.
	set PATH=%PATH%;c:\MinGW\msys\1.0\bin

	REM perform the build
	cmd /c bin\phpsdk_setvars.bat
	pushd phpdev\vc14\x64\php-src
	cmd /c buildconf --force
	cmd /c configure --disable-all --enable-cli --with-extra-includes=c:\projects\libhttp_message --with-extra-libs=c:\projects\libhttp_message --with-psr=shared --with-http_message=shared
	nmake
	popd

	REM TODO:debugging
	dir php_http_message.dll /s
	dir php.exe /s
	dir php*.dll /s

endlocal

