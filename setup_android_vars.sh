
if [[ "${ANDROID_SDK_ROOT+x}" = "x" ]]; then
    echo "using preexisting sdk values"
    export SDK=$ANDROID_SDK_ROOT
else
    echo "using default sdk values"
    export ANDROID_SDK_ROOT=/usr/lib/android_sdk
    export SDK=/usr/lib/android-sdk
fi

export NDK_VERSION=$(ls -1 $ANDROID_SDK_ROOT/ndk 2>/dev/null | sort -V | tail -n1)
export BUILD_TOOLS_VERSION=$(ls -1 $ANDROID_SDK_ROOT/build-tools 2>/dev/null | sort -V | tail -n1)
export NDK=$ANDROID_SDK_ROOT/ndk/$NDK_VERSION
export NDK_ROOT=$NDK

echo "NDK version: " $NDK_VERSION
echo "Build Tools version: " $NDK_VERSION

export ANDROID_SDK=$SDK
export ANDROID_NDK=$NDK

export ANDROID_API=26
export APP_PLATFORM=android-$ANDROID_API

export APK_NAME=Fightable
export PACKAGE_NAME=dogotrigger.fightable

# export ANDROID_HOME=$ANDROID_SDK

export BUILD_TOOLS=$ANDROID_SDK_ROOT/build-tools/$BUILD_TOOLS_VERSION

export BUILD_TYPE=RelWithDebInfo
