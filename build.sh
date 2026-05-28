set -xe

CC=x86_64-w64-mingw32-gcc
CPPC=x86_64-w64-mingw32-g++

BUILD_FLAGS="-Wformat -fPIC -O0 -g -Iminhook_prebuilt/include"
LINK_FLAGS="-static -shared -Wl,-Bdynamic -Lminhook_prebuilt/bin -lMinHook.x64 -Wl,-Bstatic -lstdc++"

C_SRC="log dxgi"
CPP_SRC="main"

OBJS=""

for f in $C_SRC
do
	$CC $BUILD_FLAGS -c ${f}.c -o ${f}.o
	OBJS="$OBJS ${f}.o"
done

for f in $CPP_SRC
do
	$CC $BUILD_FLAGS -c ${f}.cpp -o ${f}.o
	OBJS="$OBJS ${f}.o"
done

$CPPC $LINK_FLAGS $OBJS -o dxgi.dll

cp minhook_prebuilt/bin/MinHook.x64.dll ./
