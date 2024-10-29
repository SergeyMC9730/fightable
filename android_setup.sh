#!/usr/bin/bash

export APK_LIBS="lib"

export SDK=$HOME/android_sdk
export NDK=$HOME/android_ndk

export ANDROID_SDK=$SDK
export ANDROID_NDK=$NDK

export ANDROID_API=26

export APK_NAME=Fightable
export PACKAGE_NAME=dogotrigger.fightable

export ANDROID_HOME=$ANDROID_SDK

export BUILD_TOOLS=$ANDROID_HOME/build-tools/29.0.3

mkdir -p bin

$BUILD_TOOLS/aapt package -f -I "${ANDROID_SDK}/platforms/android-$ANDROID_API/android.jar" -M AndroidManifest.xml -A src/assets -S res -m -F bin/app-unsigned.apk

LIBS=$(find $APK_LIBS -type f -name '*.so' | sed 's/\\/\//g')

$BUILD_TOOLS/aapt add bin/app-unsigned.apk $(echo ${LIBS//.\//})
$BUILD_TOOLS/zipalign -v -f 4 bin/app-unsigned.apk bin/app.apk

if ! [ -e "bin/.keystore" ] ; then
    keytool -genkeypair -v -keystore bin/.keystore -keyalg RSA -keysize 2048 -validity 10000
fi

$BUILD_TOOLS/apksigner sign --ks bin/.keystore --min-sdk-version $ANDROID_API bin/app.apk