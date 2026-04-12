New-Item -Path "./build" -ItemType Directory
cmake -G Ninja -S . -B build -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl "-DOPENMPT_LIB=${env:GITHUB_WORKSPACE}/extlib/lib/libopenmpt.lib" "-DMICROHTTPD_LIB=${env:GITHUB_WORKSPACE}/extlib/lib/libmicrohttpd.lib" "-DOPENMPT_INC=${env:GITHUB_WORKSPACE}/extlib/inc" "-DMICROHTTPD_INC=${env:GITHUB_WORKSPACE}/extlib/inc" -DCMAKE_TOOLCHAIN_FILE="${env:VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DENABLE_CUSTOM_COMPILER_FLAGS=OFF
cmake -G Ninja -S . -B build -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl "-DOPENMPT_LIB=${env:GITHUB_WORKSPACE}/extlib/lib/libopenmpt.lib" "-DMICROHTTPD_LIB=${env:GITHUB_WORKSPACE}/extlib/lib/libmicrohttpd.lib" "-DOPENMPT_INC=${env:GITHUB_WORKSPACE}/extlib/inc" "-DMICROHTTPD_INC=${env:GITHUB_WORKSPACE}/extlib/inc" -DCMAKE_TOOLCHAIN_FILE="${env:VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DENABLE_CUSTOM_COMPILER_FLAGS=OFF
cmake --build build

New-Item -Path "./winout" -ItemType Directory
Copy-Item -Path "./build/fightable.exe" -Destination "./winout/"
Copy-Item -Path "./build/fightable-simple-window.exe" -Destination "./winout/"
Copy-Item -Path "./build/anim_editor.exe" -Destination "./winout/"
Copy-Item -Path "./build/keyboard-test.exe" -Destination "./winout/"
Copy-Item -Path "./build/collide-test.exe" -Destination "./winout/"
Copy-Item -Path "./build/physics-test.exe" -Destination "./winout/"
Copy-Item -Path "./build/gun-test.exe" -Destination "./winout/"
Copy-Item -Path "./build/fightable-simple-window.exe" -Destination "./winout/"
Copy-Item -Path "./build/assets" -Destination "./winout/" -Recurse
Copy-Item -Path "./extlib/lib/libmicrohttpd-dll.dll" -Destination "./winout/"
Copy-Item -Path "./extlib/lib/libopenmpt.dll" -Destination "./winout/"
Copy-Item -Path "./extlib/lib/openmpt-mpg123.dll" -Destination "./winout/"
Copy-Item -Path "./extlib/lib/openmpt-ogg.dll" -Destination "./winout/"
Copy-Item -Path "./extlib/lib/openmpt-vorbis.dll" -Destination "./winout/"
Copy-Item -Path "./extlib/lib/openmpt-zlib.dll" -Destination "./winout/"
