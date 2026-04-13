#!/usr/bin/bash

export APK_LIBS="lib"

source setup_android_vars.sh

mkdir -pv bin

echo $BUILD_TOOLS
echo "-------"
ls $BUILD_TOOLS
echo "-------"
ls -la $ANDROID_SDK
echo "-------"
ls -la $ANDROID_SDK/platforms
echo "-------"
ls -la $ANDROID_SDK/platforms/android-$ANDROID_API
echo "-------"
ls -la $ANDROID_SDK/platforms/android-26
echo "-------"

PLATDIR=$(ls -1 $ANDROID_SDK_ROOT/platforms 2>/dev/null | sort -V | head -n1)
echo $PLATDIR

$BUILD_TOOLS/aapt package -f -I "${ANDROID_SDK}/platforms/$PLATDIR/android.jar" -M AndroidManifest.xml -A src/assets -S res -m -F bin/app-unsigned.apk

LIBS=$(find $APK_LIBS -type f -name '*.so' | sed 's/\\/\//g')

$BUILD_TOOLS/aapt add bin/app-unsigned.apk $(echo ${LIBS//.\//})
$BUILD_TOOLS/zipalign -v -f 4 bin/app-unsigned.apk bin/app.apk
