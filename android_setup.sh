#!/usr/bin/bash

export APK_LIBS="lib"

source setup_android_vars.sh

mkdir -pv bin

ls $BUILD_TOOLS
ls -la $ANDROID_SDK
ls -la $ANDROID_SDK/platforms
ls -la $ANDROID_SDK/platforms/android-$ANDROID_API

PLATDIR=$(ls $ANDROID_SDK/platforms -1 | tail -n 1)
echo $PLATDIR

$BUILD_TOOLS/aapt package -f -I "${ANDROID_SDK}/platforms/$PLATDIR/android.jar" -M AndroidManifest.xml -A src/assets -S res -m -F bin/app-unsigned.apk

LIBS=$(find $APK_LIBS -type f -name '*.so' | sed 's/\\/\//g')

$BUILD_TOOLS/aapt add bin/app-unsigned.apk $(echo ${LIBS//.\//})
$BUILD_TOOLS/zipalign -v -f 4 bin/app-unsigned.apk bin/app.apk
