INCLUDEPATH += $$PWD

android {
    QT += androidextras

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    SSL_LIB = $$PWD/../../3rdparty/OpenSSL/OpenSSL/lib/android/arm
    ANDROID_EXTRA_LIBS += $$SSL_LIB/libcrypto_1_1.so $$SSL_LIB/libssl_1_1.so

    HEADERS += \
        $$PWD/androidpermissionshandler.h

    SOURCES += \
        $$PWD/androidpermissionshandler.cpp

    OTHER_FILES += \
        $$PWD/android/AndroidManifest.xml

    DISTFILES += \
        $$PWD/android/AndroidManifest.xml \
        $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/android/gradlew \
        $$PWD/android/res/values/libs.xml \
        $$PWD/android/build.gradle \
        $$PWD/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/android/gradlew.bat \
        $$PWD/android/AndroidManifest.xml \
        $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/android/gradlew \
        $$PWD/android/res/values/libs.xml \
        $$PWD/android/build.gradle \
        $$PWD/android/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/android/gradlew.bat
}
