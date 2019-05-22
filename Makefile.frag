PHP_TEST_SHARED_EXTENSIONS =  ` \
	echo "-d extension=$(EXTENSION_DIR)/psr.so"; \
	if test "x$(PHP_MODULES)" != "x"; then \
		for i in $(PHP_MODULES)""; do \
			. $$i; $(top_srcdir)/build/shtool echo -n -- " -d extension=$$dlname"; \
		done; \
	fi; \
	if test "x$(PHP_ZEND_EX)" != "x"; then \
		for i in $(PHP_ZEND_EX)""; do \
			. $$i; $(top_srcdir)/build/shtool echo -n -- " -d $(ZEND_EXT_TYPE)=$(top_builddir)/modules/$$dlname"; \
		done; \
	fi`

mrproper: clean
	rm -rf autom4te.cache build modules vendor
	rm -f acinclude.m4 aclocal.m4 config.guess config.h config.h.in config.log config.nice config.status config.sub \
		configure configure.ac install-sh libtool ltmain.sh Makefile Makefile.fragments Makefile.global \
		Makefile.objects missing mkinstalldirs run-tests.php

