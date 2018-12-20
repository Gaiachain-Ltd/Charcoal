# How to build QZXing {#how_to_build}

For barcode recognition you should build [QZXing](https://github.com/ftylitak/qzxing) library first.

In **patches** directory you can find 2 patches.
1. qzxing_build.patch - fixed github master branch to be able to build it.
2. qzxing.patch - added barcode region detection logic.

If you want to use just barcode scanning you need to apply only qzxing_build.patch. To activate barcode region detection you should apply both qzxing_build.patc and qzxing.patch (e.g. patch -s -p0 < qzxing.patch).

# How to build

1. Apply suitable patch(es).
2. Export environment variable before running the script: export ANDROID_NDK_TOOLCHAIN_VERSION=4.9.
3. Run script 
```
./scripts/build_barcode_libraries.sh 
```
Example: 
```
*./scripts/build_barcode_libraries.sh -p /ABSOLUTE_PATH_TO_ZXING_SOURCES/ -n QZXing -d ../Desktop -a ../Android -l /opt/Qt5.7.0_android/5.7/gcc_64/bin -m /opt/Qt5.7.0_android/5.7/android_armv7/bin/ -s /opt/android-sdk/ -k /opt/android-ndk*
```