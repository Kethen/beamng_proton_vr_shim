#include <windows.h>

#include "redirect.h"

REDIRECT(CreateDXGIFactory, HRESULT __stdcall, (REFIID riid, void **ppFactory), (riid, ppFactory))
REDIRECT(CreateDXGIFactory1, HRESULT __stdcall, (REFIID riid, void **ppFactory), (riid, ppFactory))
REDIRECT(CreateDXGIFactory2, HRESULT __stdcall, (UINT Flags, REFIID riid, void **ppFactory), (Flags, riid, ppFactory))
REDIRECT(DXGIDeclareAdapterRemovalSupport, HRESULT __stdcall, (), ())
REDIRECT(DXGIGetDebugInterface1, HRESULT __stdcall, (UINT Flags, REFIID riid, void **ppDebug), (Flags, riid, ppDebug))

void dxgi_fetch_prog(){
	FETCH_REDIRECT_WIN("dxgi.dll", CreateDXGIFactory);
	FETCH_REDIRECT_WIN("dxgi.dll", CreateDXGIFactory1);
	FETCH_REDIRECT_WIN("dxgi.dll", CreateDXGIFactory2);
	FETCH_REDIRECT_WIN("dxgi.dll", DXGIDeclareAdapterRemovalSupport);
	FETCH_REDIRECT_WIN("dxgi.dll", DXGIGetDebugInterface1);
}
