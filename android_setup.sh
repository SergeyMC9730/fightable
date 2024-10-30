#!/usr/bin/bash

export APK_LIBS="lib"

source setup_android_vars.sh

mkdir -p bin

$BUILD_TOOLS/aapt package -f -I "${ANDROID_SDK}/platforms/android-$ANDROID_API/android.jar" -M AndroidManifest.xml -A src/assets -S res -m -F bin/app-unsigned.apk

LIBS=$(find $APK_LIBS -type f -name '*.so' | sed 's/\\/\//g')

$BUILD_TOOLS/aapt add bin/app-unsigned.apk $(echo ${LIBS//.\//})
$BUILD_TOOLS/zipalign -v -f 4 bin/app-unsigned.apk bin/app.apk

if ! [ -e "bin/.keystore" ] ; then
    keytool -genkeypair -v -keystore bin/.keystore -keyalg RSA -keysize 2048 -validity 10000
fi

$BUILD_TOOLS/apksigner sign --ks bin/.keystore --min-sdk-version $ANDROID_API bin/app.apk