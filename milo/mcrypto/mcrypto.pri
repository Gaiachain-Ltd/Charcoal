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
    #message("MCrypto: using OpenSSL")
    DEFINES += OPENSSL_INCLUDED
} else {
    message("MCrypto: using default backend (not OpenSSL). Warning: it has not undergone security audit!")
}
