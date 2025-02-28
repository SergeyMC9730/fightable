source setup_android_vars.sh

ABI=arm64-v8a

setup_project() {
    local BPATH=build_android_$ABI

    mkdir $BPATH -p
    cd $BPATH

    # rm * -rf
    cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DDISABLE_MP_SERVER=0 ..
    cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DDISABLE_MP_SERVER=0 ..

    cd ..
}

setup_project

ABI=armeabi-v7a
setup_project

ABI=x86
setup_project

ABI=x86_64
setup_project
