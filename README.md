[Gaiachain](https://gaiachain.io/)

Proof of concept v2

# Intro

This is the second version of proof of concept application for Gaiachain project.
Some more information about the project concept can be found on [the wiki page](https://redmine.milosolutions.com/projects/gaiachain-website/wiki).

#### Second version info

There is still lots of old code for previous version in the project. Slowly, with the implementation of new UI we will get rid of it.

## Configuration

Recommended Qt version for the project is the newest LTS version (currently **Qt 5.12.5**).

### 3rd party libraries 

There are two 3rd party libraries used in the project
* [qzxing](https://github.com/ftylitak/qzxing) - used in QR code scanner (sources included in the project structure)
* [OpenSSL] - which need to be built separately, especially for Android platform.

#### OpenSSL

To build OpenSSL libraries for Android use scripts located in *Gaiachain/3rdparty/OpenSSL* directory.

1. Copy *setenv-android.sh.example* to *setenv-android.sh* and adjust all properties.
2. Run *prepare_openssl_android.sh*

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
