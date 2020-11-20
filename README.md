# [GAIACHAIN](https://gaiachain.io/)

We employ blockchain and fintech to build traceability and payment solutions that maximize incentives while minimizing costs to forest restoration and conservation. We are developing a new open source application that addresses challenges related to payment distribution and efficiency in the context of PES (payment for ecological services) and sustainable supply chains. Stay tuned for updates and get in touch if you would like to learn more!

## Configuration

Recommended Qt version for the project is **Qt 5.14.2**.

### Build

In order to build Gaiachain Cocoa, run these commands:
```
qmake CONFIG+=cocoa app_storage_password="ABCDEF"
```

And Charcoal:
```
qmake CONFIG+=charcoal app_storage_password="ABCDEF"
```

Make sure to supply a strong and preferrably long password.

### 3rd party libraries 

There are two 3rd party libraries used in the project
* [qzxing](https://github.com/ftylitak/qzxing) - used in QR code scanner (sources included in the project structure)
* [OpenSSL] - needs to be built separately. To do it, copy `setenv-android.sh.example` into `setenv-android.sh`, modify it to add relevant Android paths and newest OpenSSL version. Then you can use `prepare_openssl_android.sh` and `prepare_openssl_linux.sh` to compile. Scripts do everything automatically, and `openssl.pri` will pick this build up automatically as well

## Source info

In the main project directory, there are 4 directories:
*  doc - the documentation
*  Gaiachain - all project sources, resources, etc.
*  milo - sources for [MCDB](https://github.com/milosolutions/newprojecttemplate) modules
*  tests - sources for tests (no real unit-/ other tests for POC, use this only if you need to test something)

Inside **Gaiachain** directory, there 5 more directories:
* 3rdparty - all 3rd party libraries binaries, libraries, sources, etc. should get in there
* platforms - with platform specific files and configurations
* qml - qml source files
* resource - for all the resources (graphics, fonts, etc.)
* src - cpp/h source files
