NDK_VERSION=28.0.13004108
BUILD_TOOLS_VERSION=29.0.3
PLATFORM_VERSION=android-26
CMAKE_VERSION=3.31.5

echo "* cores:" $(nproc)

yes | sudo sdkmanager --licenses
yes | sudo sdkmanager --update
yes | sudo sdkmanager "platforms;$PLATFORM_VERSION" "build-tools;$BUILD_TOOLS_VERSION" "ndk;$NDK_VERSION" "platform-tools" "cmdline-tools;latest" "cmake;$CMAKE_VERSION"

TARGET_REPO=$1

cd $TARGET_REPO
source setup_android_vars.sh

export PATH=/usr/lib/android-sdk/ndk/$NDK_VERSION:$PATH

mkdir android_req
cd android_req

wget https://lib.openmpt.org/files/libopenmpt/src/libopenmpt-0.7.13+release.makefile.tar.gz
wget https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-latest.tar.gz

tar -xf "libopenmpt-0.7.13+release.makefile.tar.gz"
tar -xf "libmicrohttpd-latest.tar.gz"

cd libopenmpt-0.7.13+release/
cp build/android_ndk/* . -rv
mkdir jni
cp * jni/ -r
ndk-build -j $(nproc)
sudo cp libs/arm64-v8a/* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/ -rv
sudo cp libs/armeabi-v7a/* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/arm-linux-androideabi/ -rv
sudo cp libs/x86/* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/i686-linux-android/ -rv
sudo cp libs/x86_64/* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/x86_64-linux-android/ -rv
sudo mkdir -p $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/libopenmpt
sudo cp libopenmpt/*.h* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/libopenmpt/ -rv

cd ..
cd libmicrohttpd-1.0.1/

export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64
export TARGET=aarch64-linux-android
export API=$ANDROID_API
export AR=$TOOLCHAIN/bin/llvm-ar
export CC="$TOOLCHAIN/bin/clang --target=$TARGET$API"
export AS=$CC
export CXX="$TOOLCHAIN/bin/clang++ --target=$TARGET$API"
export LD=$TOOLCHAIN/bin/ld
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
export STRIP=$TOOLCHAIN/bin/llvm-strip
./configure --host $TARGET --prefix=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr
make -j$(nproc)
sudo make install
sudo cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/ -rv
sudo rm $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* -rv

make clean
export TARGET=armv7a-linux-androideabi
export CC="$TOOLCHAIN/bin/clang --target=$TARGET$API"
export CXX="$TOOLCHAIN/bin/clang++ --target=$TARGET$API"
./configure --host $TARGET --prefix=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr
make -j$(nproc)
sudo make install
sudo cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/arm-linux-androideabi/ -rv
sudo rm $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* -rv

make clean
export TARGET=i686-linux-android
export CC="$TOOLCHAIN/bin/clang --target=$TARGET$API"
export CXX="$TOOLCHAIN/bin/clang++ --target=$TARGET$API"
./configure --host $TARGET --prefix=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr
make -j$(nproc)
sudo make install
sudo cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/i686-linux-android/ -rv
sudo rm $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* -rv

make clean
export TARGET=x86_64-linux-android
export CC="$TOOLCHAIN/bin/clang --target=$TARGET$API"
export CXX="$TOOLCHAIN/bin/clang++ --target=$TARGET$API"
./configure --host $TARGET --prefix=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr
make -j$(nproc)
sudo make install
sudo cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/x86_64-linux-android/ -rv
sudo rm $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* -rv

cd $TARGET_REPO
source setup_android_builds.sh
source compile_android.sh

cd gh
tr -dc A-Za-z0-9 < /dev/random | (head -c 13; echo) > rand.txt
cat rand.txt rand.txt keystore.txt rand.txt > newkeystore.txt
cp newkeystore.txt ../
cd ..

cat newkeystore.txt | bash android_setup.sh

cd bin
mv app.apk fightable.apk