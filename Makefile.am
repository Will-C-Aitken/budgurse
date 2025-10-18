AM_CFLAGS = -Wall
AM_CPPFLAGS = -I$(srcdir)/src -I$(srcdir)/deps

bin_PROGRAMS = budgurse

budgurse_SOURCES = \
	deps/status.h \
	src/budgurse.c \
	src/budgurse.h \
	src/entries.c \
	src/entries.h \
	src/backend.c \
	src/backend.h \
	src/browser.c \
	src/browser.h \
	src/categories.c \
	src/categories.h \
	src/global.h \
	src/help.c \
	src/help.h \
	src/llist.c \
	src/llist.h \
	src/prompt.c \
	src/prompt.h \
	src/summary.c \
	src/summary.h \
	src/util.c \
	src/util.h \
	src/wins.c \
	src/wins.h

TESTS = unit_tests
check_PROGRAMS = unit_tests
unit_tests_SOURCES = \
	deps/status.h \
	test/test.c \
	test/test.h \
	test/backend_tests.c \
	test/browser_tests.c \
	test/categories_tests.c \
	test/entries_tests.c \
	test/llist_tests.c \
	test/prompt_tests.c \
	test/summary_tests.c \
	test/misc_tests.c \
	test/test_utils.c \
	src/backend.c \
	src/backend.h \
	src/browser.c \
	src/browser.h \
	src/budgurse.h \
	src/categories.c \
	src/categories.h \
	src/entries.c \
	src/entries.h \
	src/global.h \
	src/help.c \
	src/help.h \
	src/llist.c \
	src/llist.h \
	src/prompt.c \
	src/prompt.h \
	src/summary.c \
	src/summary.h \
	src/util.c \
	src/util.h \
	src/wins.c \
	src/wins.h
