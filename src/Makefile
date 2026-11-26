BUILDDIR := ..\build

TESTDIR := tests

CHAT_TEST_SRC := $(TESTDIR)\chatTest.c chat\user.c
chatTests := $(BUILDDIR)\chatTests

SETUP_TEST_SRC := $(TESTDIR)\setupTest.c LAN\setup.c
setupTests := $(BUILDDIR)\setupTests

all: $(chatTests) $(setupTests)

setuptest: $(setupTests)

chattest: $(chatTests)

$(chatTests): $(CHAT_TEST_SRC) | $(BUILDDIR)
	gcc $^ -o $@

$(setupTests): $(SETUP_TEST_SRC) | $(BUILDDIR)
	gcc $^ -o $@ -l ws2_32

$(BUILDDIR):
	mkdir $(BUILDDIR)

