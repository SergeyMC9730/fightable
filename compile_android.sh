source setup_android_vars.sh

ABI=arm64-v8a
BASE_LIB_PATH=$PWD/lib

mkdir -p $BASE_LIB_PATH

compile_project() {
    local LABI=$ABI
    local BPATH=build_android_$LABI
    local LIB_PATH=$BASE_LIB_PATH/$LABI

    mkdir -p $LIB_PATH

    cd $BPATH
    cmake --build . -j$(nproc)
    cp libfightable.so $LIB_PATH -v
    cp $(find . | grep "libportaudio") $LIB_PATH -v
    cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/libmicrohttpd.so $LIB_PATH -v
    cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/libopenmpt.so $LIB_PATH -v
    cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/libc++_shared.so $LIB_PATH -v
    cd ..
}

SYSROOT_LIB=aarch64-linux-android
compile_project

ABI=armeabi-v7a
SYSROOT_LIB=arm-linux-androideabi
compile_project

ABI=x86
SYSROOT_LIB=i686-linux-android
compile_project

ABI=x86_64
SYSROOT_LIB=x86_64-linux-android
compile_project
