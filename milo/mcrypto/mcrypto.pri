isEmpty(OPENSSL_PATH) {
  win32:OPENSSL_PATH=$$PWD/OpenSSL-Win32
  unix:OPENSSL_PATH=/usr/include/openssl
  android:OPENSSL_PATH=$$PWD/scripts/OpenSSL/android/armeabi-v7a
}

isEmpty(OPENSSL_INCLUDE) {
  unix:OPENSSL_INCLUDE=/usr/include/openssl
}

win32 {
    exists($$OPENSSL_PATH/include/*) {
        LIBS += -L$$OPENSSL_PATH/lib -lssleay32 -llibeay32
        INCLUDEPATH += $$OPENSSL_PATH/include
        CONFIG += openssl
    }
}

android {
    exists($$OPENSSL_PATH/include/*) {
        INCLUDEPATH += $$OPENSSL_PATH/include
        # TODO: separate paths per architecture!
        LIBS += -L$$OPENSSL_PATH/lib/android/arm -lssl_1_1 -lcrypto_1_1
        CONFIG += openssl
    }
} else {
    unix {
        exists($$OPENSSL_PATH/*) {
            LIBS += -lssl -lcrypto
            INCLUDEPATH += $$OPENSSL_INCLUDE
            CONFIG += openssl
        }
    }
}

INCLUDEPATH += $$PWD

SOURCES += $$PWD/mcrypto.cpp \
    $$PWD/qaesencryption.cpp

HEADERS += $$PWD/mcrypto.h \
    $$PWD/qaesencryption.h

DEFINES += MCRYPTO_LIB

no-openssl {
    CONFIG -= openssl
}

openssl {
    message("MCrypto: using OpenSSL")
    DEFINES += OPENSSL_INCLUDED
} else {
    message("MCrypto: using default backend (not OpenSSL). Warning: it has not undergone security audit!")
}
