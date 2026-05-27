set -xe

x86_64-w64-mingw32-g++ -Wformat -fPIC -static -shared -Iminhook_prebuilt/include main.cpp -Lminhook_prebuilt/bin/ -Wl,-Bdynamic -lMinHook.x64 -Wl,-Bstatic -lstdc++ -o dxgi.dll
cp minhook_prebuilt/bin/MinHook.x64.dll ./
