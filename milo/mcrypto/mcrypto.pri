isEmpty(OPENSSL_PATH) {
  win32:OPENSSL_PATH=$$PWD/OpenSSL-Win32
  unix:OPENSSL_PATH=$$PWD/scripts/OpenSSL/linux
  android:OPENSSL_PATH=$$PWD/scripts/OpenSSL/android/armeabi-v7a
}

win32 {
    exists($$OPENSSL_PATH/include/*) {
        LIBS += -L$$OPENSSL_PATH/lib -lssleay32 -llibeay32
        CONFIG += openssl
    }
}

android {
    exists($$OPENSSL_PATH/include/*) {
        OSSL_ARCH=""
        equals(QT_ARCH, "armeabi-v7a") {
            OSSL_ARCH="arm"
        }

        equals(QT_ARCH, "arm64-v8a") {
            OSSL_ARCH="arm64"
        }

        equals(QT_ARCH, "x86") {
            OSSL_ARCH="x86"
        }

        equals(QT_ARCH, "x86_64") {
            OSSL_ARCH="x86_64"
        }

        LIBS += -L$$OPENSSL_PATH/lib/android/$${OSSL_ARCH} -lssl_1_1 -lcrypto_1_1
        CONFIG += openssl
    }
} else {
    unix {
        exists($$OPENSSL_PATH/*) {
            LIBS += -L$$OPENSSL_PATH/lib/linux -lssl -lcrypto
            CONFIG += openssl
        }
    }
}

INCLUDEPATH += $$PWD $$OPENSSL_PATH/include

SOURCES += $$PWD/mcrypto.cpp \
    $$PWD/qaesencryption.cpp

HEADERS += $$PWD/mcrypto.h \
    $$PWD/qaesencryption.h

DEFINES += MCRYPTO_LIB

no-openssl {
    CONFIG -= openssl
}

openssl {
    #message("MCrypto: using OpenSSL")
    DEFINES += OPENSSL_INCLUDED
} else {
    message("MCrypto: using default backend (not OpenSSL). Warning: it has not undergone security audit!")
}
