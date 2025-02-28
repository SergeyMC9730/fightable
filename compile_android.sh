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
    cd ..
}

compile_project

ABI=armeabi-v7a
# compile_project

ABI=x86
# compile_project

ABI=x86_64
# compile_project
