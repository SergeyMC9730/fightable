NDK_VERSION=28.0.13004108
BUILD_TOOLS_VERSION=29.0.3
PLATFORM_VERSION=android-26
CMAKE_VERSION=3.31.5

echo "* cores:" $(nproc)
echo
echo "---- COMPILING FOR $2 AT $1 ----"
echo

FTEMP=$(yes | sudo sdkmanager --licenses)
yes | sudo sdkmanager --update
yes | sudo sdkmanager "platforms;$PLATFORM_VERSION" "build-tools;$BUILD_TOOLS_VERSION" "ndk;$NDK_VERSION" "platform-tools" "cmdline-tools;latest" "cmake;$CMAKE_VERSION"

unset FTEMP

TARGET_REPO=$1
W_ARCH=$2

cd $TARGET_REPO
source setup_android_vars.sh
source translate_abi.sh $W_ARCH

echo
echo "---- TRANSLATED ABI: $W_ARCH $CLANG_TARGET $NDK_ABI $SYSROOT_LIB $W_ABI $NDK_ABI_B"
echo

export PATH=/usr/lib/android-sdk/ndk/$NDK_VERSION:$PATH

mkdir android_req
cd android_req

wget https://lib.openmpt.org/files/libopenmpt/src/libopenmpt-0.7.13+release.makefile.tar.gz &
wget https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-latest.tar.gz &
git clone https://github.com/ibaoger/libcurl-android.git --recursive &

wait

tar -xf "libopenmpt-0.7.13+release.makefile.tar.gz" &
tar -xf "libmicrohttpd-latest.tar.gz" &

wait

export PATH=$PATH:$ANDROID_NDK
which ndk-build

compile_mpt() {
    echo "mpt: start date: $(date)"

	cd libopenmpt-0.7.13+release/
	cp build/android_ndk/* . -rv
	mkdir jni
	cp * jni/ -r
	if ndk-build -j $(nproc) ; then
		echo MPT GOOD
	else
	    echo MPT BAD
		exit 1
	fi
	echo $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib
	ls $ANDROID_SDK/ndk -la
	ls $ANDROID_SDK/ -la
	ls -la $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib
	mkdir -pv $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/
	sudo cp libs/$W_ARCH/* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -rv
	sudo mkdir -pv $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/libopenmpt
	sudo cp libopenmpt/*.h* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/libopenmpt/ -rv

	echo "mpt: end date: $(date)"
}

compile_mhd() {
    echo "mhd: start date: $(date)"

	cd libmicrohttpd-1.0.5

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
		echo MHD GOOD
	else
		exit 1
	fi

	if make -j$(nproc) ; then
		echo MHD GOOD
	else
	    echo MHD BAD
		exit 1
	fi
	sudo make install
	sudo cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -rv
	sudo rm $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/libmicro* -rv

	echo "mhd: end date: $(date)"
}

compile_curl() {
    echo "curl: start date: $(date)"

    cd libcurl-android

    chmod 755 build_for_android.sh
    ./build_for_android.sh

    sleep 3

    ls libs -la
    ls jni/build/zlib -la
    ls jni/build/openssl -la

    sudo cp jni/curl/include/curl $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/ -rv
    sudo cp libs/$NDK_ABI_B/libcurl.so $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v
    sudo cp jni/build/zlib/$NDK_ABI_B/lib/libz.so $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v
    sudo cp jni/build/zlib/$NDK_ABI_B/lib/libz.a $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v
    sudo cp jni/build/openssl/$NDK_ABI_B/lib/libssl.a $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v
    sudo cp jni/build/openssl/$NDK_ABI_B/lib/libssl.so $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v
    sudo cp jni/build/openssl/$NDK_ABI_B/lib/libcrypto.a $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v
    sudo cp jni/build/openssl/$NDK_ABI_B/lib/libcrypto.so $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/ -v

    echo "curl: end date: $(date)"
}

mkdir -p $TARGET_REPO/logs

(compile_mpt | tee -a $TARGET_REPO/logs/compile_mpt.txt) &
(compile_mhd | tee -a $TARGET_REPO/logs/compile_mhd.txt) &
(compile_curl | tee -a $TARGET_REPO/logs/compile_curl.txt) &

wait

cd $TARGET_REPO
source setup_android_build.sh $W_ABI $W_ABI | tee -a $TARGET_REPO/logs/compile_fightable_a.txt
source compile_android_arch.sh $W_ABI $W_ABI | tee -a $TARGET_REPO/logs/compile_fightable_b.txt
