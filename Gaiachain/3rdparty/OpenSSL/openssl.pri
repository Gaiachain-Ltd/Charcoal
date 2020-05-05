OSSL_PATH = $$PWD/OpenSSL

win32 {
    exists($${OSSL_PATH}/include/*) {
        LIBS += -L$${OSSL_PATH}/lib -lssleay32 -llibeay32
        CONFIG += openssl
    } else {
        error("OpenSSL has not been found in $$OSSL_PATH")
    }
}

android {
    exists($${OSSL_PATH}/include/*) {
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

        OSSL_LIBS=$${OSSL_PATH}/lib/android/$${OSSL_ARCH}
        LIBS += -L$${OSSL_LIBS} -lssl_1_1 -lcrypto_1_1
        ANDROID_EXTRA_LIBS += $${OSSL_LIBS}/libcrypto_1_1.so $${OSSL_LIBS}/libssl_1_1.so
        CONFIG += openssl
    }
} else {
    unix {
        exists($${OSSL_PATH}/*) {
            LIBS += -L$${OSSL_PATH}/lib/linux -lssl -lcrypto
            CONFIG += openssl
        }
    }
}

INCLUDEPATH += $${OSSL_PATH}/include
