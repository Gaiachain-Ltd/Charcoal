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

        OSSL_AND=$${OSSL_PATH}/lib/android
        OSSL_LIBS=$${OSSL_AND}/$${OSSL_ARCH}
        LIBS += -L$${OSSL_LIBS} -lssl_1_1 -lcrypto_1_1
        CONFIG += openssl

        # Copies the given files to the destination directory
        # Src: https://stackoverflow.com/questions/3984104/qmake-how-to-copy-a-file-to-the-output
        defineTest(copySsl) {
            from = $$1
            to = $$2

            # Replace slashes in paths with backslashes for Windows
            win32:from ~= s,/,\\,g
            win32:to ~= s,/,\\,g

            QMAKE_POST_LINK += rm -rf $$quote($$to) $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += mkdir -p $$quote($$to) $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$from/libcrypto_1_1.so) $$quote($$to) $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$from/libssl_1_1.so) $$quote($$to) $$escape_expand(\\n\\t)

            export(QMAKE_POST_LINK)
        }

        AND_LIBS=$$PWD/../../platforms/android/android/libs

        # Copy OpenSSL libs to special Android folders because
        # ANDROID_EXTRA_LIBS does not work anymore!
        copySsl($${OSSL_AND}/arm, $${AND_LIBS}/armeabi-v7a)
        copySsl($${OSSL_AND}/arm64, $${AND_LIBS}/arm64-v8a)
        copySsl($${OSSL_AND}/x86, $${AND_LIBS}/x86)
        copySsl($${OSSL_AND}/x86_64, $${AND_LIBS}/x86_64)
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
