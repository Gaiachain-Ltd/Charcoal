CONFIG += tests

# here because of QTBUG-48416
include(Gaiachain/translations/translations.pri)

TEMPLATE = subdirs

docs {
    message("Updating doxygen file. No compilation will be performed!")
    doxy.input = $$PWD/Gaiachain.doxyfile.in
    doxy.output = $$PWD/Gaiachain.doxyfile
    QMAKE_SUBSTITUTES += doxy
} else {
    SUBDIRS += Gaiachain \

    tests {
        !android {
            CONFIG(debug, debug|release) {
                #message("Running test suite")
                SUBDIRS += tests \
            }
        }
    }
}
