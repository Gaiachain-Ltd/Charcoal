INCLUDEPATH += $$PWD

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    $$PWD/android/AndroidManifest.xml

android {
    QT += androidextras

    SSL_LIB = $$PWD/../../3rdparty/OpenSSL/OpenSSL/lib/android/arm
    ANDROID_EXTRA_LIBS += $$SSL_LIB/libcrypto_1_1.so $$SSL_LIB/libssl_1_1.so
}
