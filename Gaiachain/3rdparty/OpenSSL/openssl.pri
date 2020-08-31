isEmpty(OSSL_PATH) {
    OSSL_PATH = $$PWD/OpenSSL
}

#win32 {
#    exists($${OSSL_PATH}/include/*) {
#        LIBS += -L$${OSSL_PATH}/lib -lssleay32 -llibeay32
#        CONFIG += openssl
#    } else {
#        error("OpenSSL has not been found in $$OSSL_PATH")
#    }
#}

android {
    exists($${OSSL_PATH}/include/*) {
        # Copies the given files to the destination directory
        # Src: https://stackoverflow.com/questions/3984104/qmake-how-to-copy-a-file-to-the-output
        # We copy OpenSSL libs to special Android folders because
        # ANDROID_EXTRA_LIBS does not work reliably anymore!
        defineTest(copySsl) {
            from = $$1
            to = $$2

            # Replace slashes in paths with backslashes for Windows
            win32:from ~= s,/,\\,g
            win32:to ~= s,/,\\,g

            #QMAKE_POST_LINK += rm -rf $$quote($$to) $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += mkdir -p $$quote($$to) $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$from/libcrypto_1_1.so) $$quote($$to) $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$from/libssl_1_1.so) $$quote($$to) $$escape_expand(\\n\\t)

            export(QMAKE_POST_LINK)
        }

        OSSL_AND=$${OSSL_PATH}/lib/android
        AND_LIBS=$$PWD/../../platforms/android/android/libs
        QMAKE_POST_LINK += rm -rf $$quote($${AND_LIBS}) $$escape_expand(\\n\\t)

        OSSL_ARCH=""
        equals(QT_ARCH, "armeabi-v7a") {
            OSSL_ARCH="arm"
            copySsl($${OSSL_AND}/arm, $${AND_LIBS}/armeabi-v7a)
        }

        equals(QT_ARCH, "arm64-v8a") {
            OSSL_ARCH="arm64"
            copySsl($${OSSL_AND}/arm64, $${AND_LIBS}/arm64-v8a)
        }

        equals(QT_ARCH, "x86") {
            OSSL_ARCH="x86"
            copySsl($${OSSL_AND}/x86, $${AND_LIBS}/x86)
        }

        equals(QT_ARCH, "x86_64") {
            OSSL_ARCH="x86_64"
            copySsl($${OSSL_AND}/x86_64, $${AND_LIBS}/x86_64)
        }

        OSSL_LIBS=$${OSSL_AND}/$${OSSL_ARCH}
        LIBS += -L$${OSSL_LIBS} -lssl_1_1 -lcrypto_1_1
        CONFIG += openssl
    }
} else {
    unix {
        exists($${OSSL_PATH}/*) {
            LIBS += -L$${OSSL_PATH}/lib/linux/linux -lssl -lcrypto
            CONFIG += openssl
        }
    }

    win32 {
        exists($${OSSL_PATH}/*) {
            LIBS += -L$${OSSL_PATH}/lib -lssl -lcrypto
            CONFIG += openssl
        }
    }
}

INCLUDEPATH += $${OSSL_PATH}/include
