QT += core gui widgets qml quick multimedia concurrent

include(../mbarcodescanner.pri)

TARGET = barcode_scanner

TEMPLATE = app

SOURCES += main.cpp

RESOURCES += \
    ui/qml/qmlresource.qrc

# qzxing library
LIBS += -lQZXing

# Uncomment to draw barcode region
#CONFIG += draw_region

# draws barcode region (you have to rebuild qzxing library with our patch - see README)
draw_region {
    DEFINES += DRAW_REGION
}

INCLUDEPATH += # path to qzxing headers

linux:!android {
    LIBS += -L # path to qzxing library
}

android {
    LIBS += -L # path to qzxing library
    DISTFILES += android/AndroidManifest.xml
}

contains(ANDROID_TARGET_ARCH, armeabi-v7a) {
    ANDROID_EXTRA_LIBS = # path to qzxing library (with file name)
}
