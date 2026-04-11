New-Item -Path "./build" -ItemType Directory
$workspace = $env:GITHUB_WORKSPACE -replace '\\', '/'
cmake -G Ninja -S . -B build -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DOPENMPT_LIB=$workspace/extlib/lib/libopenmpt.lib -DMICROHTTPD_LIB=$workspace/extlib/lib/libmicrohttpd.lib -DOPENMPT_INC=$workspace/extlib/inc -DMICROHTTPD_INC=$workspace/extlib/inc -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build build
