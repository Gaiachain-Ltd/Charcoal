# Provide version number to application
COMMIT = $$system(git rev-parse --short HEAD)

VERSION=0.1.8

HOST_MACHINE = $$[QMAKE_SPEC]
contains (HOST_MACHINE, .*win32.*) {
    BUILD_DATE=$$system(powershell -Command "Get-Date -format yyyy-MM-dd")
} else {
    BUILD_DATE = $$system(date +%Y-%m-%d)
}


cocoa {
    APP_NAME = "Gaiachain Cocoa"
    APP_DOMAIN = "com.milosolutions.gaiachain"
    #"cocoa.gaiachain.io"
}

charcoal {
    APP_NAME = "Gaiachain Charcoal"
    APP_DOMAIN = "charcoal.gaiachain.io"
}

APP_VERSION = "$$VERSION"
COMPANY_NAME = "Milo Solutions"
COMPANY_DOMAIN = "milosolutions.com"

ANDROID_VERSION_NAME = "$$VERSION"
ANDROID_VERSION_CODE = 6

# add defines
DEFINES += AppVersion='"\\\"$$APP_VERSION\\\""'
DEFINES += AppName='"\\\"$$APP_NAME\\\""'
DEFINES += AppDomain='"\\\"$$APP_DOMAIN\\\""'
DEFINES += CompanyName='"\\\"$$COMPANY_NAME\\\""'
DEFINES += CompanyDomain='"\\\"$$COMPANY_DOMAIN\\\""'
DEFINES += BuildDate=\\\"$$BUILD_DATE\\\"
DEFINES += GitCommit=\\\"$$COMMIT\\\"

# To use VERSION in non-Qt files, use QMAKE_SUBSTITUTES:
#manifest.input = $$PWD/AndroidManifest.xml.in
#manifest.output = $$PWD/Gaiachain/platforms/android/android/AndroidManifest.xml
#QMAKE_SUBSTITUTES += manifest
