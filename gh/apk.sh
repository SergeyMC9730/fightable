BPATH=$1

NDK_VERSION=28.0.13004108
BUILD_TOOLS_VERSION=29.0.3
PLATFORM_VERSION=android-26
CMAKE_VERSION=3.31.5

echo "* cores:" $(nproc)

FTEMP=$(yes | sudo sdkmanager --licenses)
yes | sudo sdkmanager --update
yes | sudo sdkmanager "platforms;$PLATFORM_VERSION" "build-tools;$BUILD_TOOLS_VERSION" "ndk;$NDK_VERSION" "platform-tools" "cmdline-tools;latest" "cmake;$CMAKE_VERSION"

unset FTEMP

source setup_android_vars.sh

ls $BPATH -la
echo ---------------
ls $BPATH/lib -la
echo ---------------
file $BPATH/lib/*

source android_setup.sh
