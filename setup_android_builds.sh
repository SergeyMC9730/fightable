source setup_android_vars.sh

ABI=arm64-v8a
BUILD_TYPE=Debug

setup_project() {
    local BPATH=build_android_$ABI

    mkdir $BPATH -p
    cd $BPATH

    # rm * -rf
    cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=Debug ..
    cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=Debug ..

    cd ..
}

setup_project

ABI=armeabi-v7a
setup_project

ABI=x86
setup_project

ABI=x86_64
setup_project
