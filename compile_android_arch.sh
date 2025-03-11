source setup_android_vars.sh
export BASE_LIB_PATH=$PWD/lib
mkdir -p $BASE_LIB_PATH

echo "* BASE_LIB_PATH=$BASE_LIB_PATH"
echo "* PWD=$PWD"
echo "* TEST=$2"

LABI=$2
BPATH=build_android_$LABI
LIB_PATH=$BASE_LIB_PATH/$LABI

echo "* LABI=$LABI"
echo "* BPATH=$BPATH"
echo "* LIB_PATH=$LIB_PATH"

source translate_abi.sh $LABI

mkdir -p $LIB_PATH

cd $BPATH
if cmake --build . -j$(nproc) ; then
	echo
else
	exit 1
fi
cp libfightable.so $LIB_PATH -v
cp $(find . | grep "libportaudio") $LIB_PATH -v
cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/libmicrohttpd.so $LIB_PATH -v
cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/libopenmpt.so $LIB_PATH -v
cp $NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/$SYSROOT_LIB/libc++_shared.so $LIB_PATH -v
cd ..
