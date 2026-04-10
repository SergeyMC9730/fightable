
if [[ "${ANDROID_SDK_ROOT+x}" = "x" ]]; then
    echo "using preexisting sdk values"
    export SDK=$ANDROID_SDK_ROOT
    export NDK=$ANDROID_SDK_ROOT/ndk/28.0.13004108
else
    echo "using default sdk values"
    export ANDROID_SDK_ROOT=/usr/lib/android_sdk
    export SDK=/usr/lib/android-sdk
    export NDK=/usr/lib/android-sdk/ndk/28.0.13004108
fi

export ANDROID_SDK=$SDK
export ANDROID_NDK=$NDK

export ANDROID_API=26
export APP_PLATFORM=android-$ANDROID_API

export APK_NAME=Fightable
export PACKAGE_NAME=dogotrigger.fightable

# export ANDROID_HOME=$ANDROID_SDK

export BUILD_TOOLS=$ANDROID_SDK_ROOT/build-tools/29.0.3

export BUILD_TYPE=RelWithDebInfo
