New-Item -Path "./build" -ItemType Directory
$workspace = $env:GITHUB_WORKSPACE -replace '\\', '/'
cmake -G Ninja -S . -B build -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DOPENMPT_LIB=$workspace/extlib/lib/libopenmpt.lib -DMICROHTTPD_LIB=$workspace/extlib/lib/libmicrohttpd.lib -DOPENMPT_INC=$workspace/extlib/inc -DMICROHTTPD_INC=$workspace/extlib/inc -DCURL_INC=$workspace/extlib/inc
cmake --build build
