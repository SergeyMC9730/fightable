NDK_VERSION=28.0.13004108
BUILD_TOOLS_VERSION=29.0.3
PLATFORM_VERSION=android-26
CMAKE_VERSION=3.31.5

echo "* cores:" $(nproc)
echo
echo "---- COMPILING FOR $2 AT $1 ----"
echo

yes | sudo sdkmanager --licenses
yes | sudo sdkmanager --update
yes | sudo sdkmanager "platforms;$PLATFORM_VERSION" "build-tools;$BUILD_TOOLS_VERSION" "ndk;$NDK_VERSION" "platform-tools" "cmdline-tools;latest" "cmake;$CMAKE_VERSION"

TARGET_REPO=$1
W_ARCH=$2

cd $TARGET_REPO
source setup_android_vars.sh
source translate_abi.sh $W_ARCH

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
if ndk-build -j $(nproc) ; then
	echo
else
	exit 1
fi
sudo cp libs/$W_ARCH/* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -rv
sudo mkdir -p $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/libopenmpt
sudo cp libopenmpt/*.h* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/libopenmpt/ -rv

cd ..
cd libmicrohttpd-1.0.1/

export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64
export TARGET=$CLANG_TARGET
export API=$ANDROID_API
export AR=$TOOLCHAIN/bin/llvm-ar
export CC="$TOOLCHAIN/bin/clang --target=$TARGET$API"
export AS=$CC
export CXX="$TOOLCHAIN/bin/clang++ --target=$TARGET$API"
export LD=$TOOLCHAIN/bin/ld
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
export STRIP=$TOOLCHAIN/bin/llvm-strip
if ./configure --host $TARGET --prefix=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr ; then
	echo
else
	exit 1
fi

if make -j$(nproc) ; then
	echo
else
	exit 1
fi
sudo make install
sudo cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -rv
sudo rm $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* -rv

cd $TARGET_REPO
source setup_android_build.sh $W_ABI
source compile_android_arch.sh $W_ABI
