QT *= concurrent multimedia
CONFIG += c++11
DEFINES *= QT_USE_QSTRINGBUILDER

HEADERS += \
    $$PWD/MImageConverters.h \
    $$PWD/MBarcodeCaptureFilter.h

SOURCES += \
    $$PWD/MImageConverters.cpp \
    $$PWD/MBarcodeCaptureFilter.cpp

OTHER_FILES += $$PWD/README.md $$PWD/barcode_scanning.doxyfile \
               $$PWD/camera_tricks.md $$PWD/how_to_build.md
