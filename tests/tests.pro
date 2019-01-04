# Main qmake configuration file for unit tests

TEMPLATE = subdirs

# Add new test cases here:
#SUBDIRS += \
#	../milo/mrestapi/tst_mrestapi \
#	../milo/mlog/tst_mlog \
#	../milo/mcrypto/tst_mcrypto \
#	../milo/mconfig/tst_mconfig \
#    tst_project


# Please use convenience includes:
# testConfig.pri - contains general configs
# helpers/helpers.pri - contains property tester, great for testing QObjects

# Tests can be run in Qt Creator (enable AutoTest plugin, then run tests from
# panel 8). Tests can be run from command line. Just run:
# $ make check
# In build directory (where root Makefile is).
