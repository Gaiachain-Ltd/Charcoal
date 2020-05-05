INCLUDEPATH += $$PWD

android {
    QT += androidextras

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    HEADERS += \
        $$PWD/androidpermissionshandler.h

    SOURCES += \
        $$PWD/androidpermissionshandler.cpp

    OTHER_FILES += \
        $$PWD/android/AndroidManifest.xml

    manifest.input = $$PWD/AndroidManifest.xml.in
    manifest.output = $$PWD/android/AndroidManifest.xml
    QMAKE_SUBSTITUTES += manifest
}
