New-Item -Path "./build" -ItemType Directory
cmake -G Ninja -S . -B build -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DOPENMPT_LIB=$env:GITHUB_WORKSPACE/extlib/lib/libopenmpt.lib -DMICROHTTPD_LIB=$env:GITHUB_WORKSPACE/extlib/lib/libmicrohttpd.lib -DOPENMPT_INC=$env:GITHUB_WORKSPACE/extlib/inc -DMICROHTTPD_INC=$env:GITHUB_WORKSPACE/extlib/inc -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build build
