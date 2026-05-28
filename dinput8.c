#include <windows.h>

#include "redirect.h"

REDIRECT(DirectInput8Create, HRESULT WINAPI, (HINSTANCE a, DWORD b, REFIID c, LPVOID *d, LPUNKNOWN e), (a, b, c, d, e))

void dinput8_fetch_prog(){
	FETCH_REDIRECT_WIN("dinput8.dll", DirectInput8Create);
}
