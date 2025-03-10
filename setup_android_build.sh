source setup_android_vars.sh

ABI=$1
BPATH=build_android_$ABI

echo "* ABI=$ABI"
echo "* BPATH=$BPATH"
echo "* TEST=$2"

mkdir $BPATH -p
cd $BPATH

# rm * -rf
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DDISABLE_MP_SERVER=0 ..
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=$ABI -DANDROID_PLATFORM=android-26 -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DDISABLE_MP_SERVER=0 ..

cd ..
