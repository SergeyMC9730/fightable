A_ABI=$1

if [ $A_ABI = "arm64-v8a" ]; then
	export NDK_ABI=aarch64
	export SYSROOT_LIB=$NDK_ABI-linux-android
	export CLANG_TARGET=$SYSROOT_LIB
fi
if [ $A_ABI = "armeabi-v7a" ]; then
	export NDK_ABI=arm
	export SYSROOT_LIB=$NDK_ABI-linux-androideabi
	export CLANG_TARGET=armv7a-linux-androideabli
fi
if [ $A_ABI = "x86" ]; then
	export NDK_ABI=i686
	export SYSROOT_LIB=$NDK_ABI-linux-android
	export CLANG_TARGET=$SYSROOT_LIB
fi
if [ $A_ABI = "x86_64" ]; then
	export NDK_ABI=x86_64
	export SYSROOT_LIB=$NDK_ABI-linux-android
	export CLANG_TARGET=$SYSROOT_LIB
fi
