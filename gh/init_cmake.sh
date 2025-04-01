echo "* initializing cmake"

TARGET_REPO=$1
BUILD_DIR=$2
echo "  * target: " $TARGET_REPO
echo "  * build directory: " $BUILD_DIR

BUILD_TYPE=RelWithDebInfo
echo "  * build type: " $BUILD_TYPE

mkdir -p $BUILD_DIR

cd $BUILD_DIR
cmake $TARGET_REPO -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake $TARGET_REPO -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_POLICY_VERSION_MINIMUM=3.5

echo "* done"
