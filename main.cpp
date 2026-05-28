#include <winsock2.h>
#include <libloaderapi.h>
#include <memoryapi.h>
#include <psapi.h>
#include <bcrypt.h>
#include <windows.h>

#include <MinHook.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <wctype.h>

#include <map>

#include "log.h"
#include "dxgi.h"

#define STR(s) #s

#define HOOK(name) { \
	int create_status = MH_CreateHook(name, name##_hooked, (void **)&name##_orig); \
	if (create_status != MH_OK){ \
		LOG("%s: failed creating hook for %s, %d\n", __func__, STR(name), create_status); \
		exit(1); \
	} \
	\
	int enable_status = MH_EnableHook(name); \
	if (enable_status != MH_OK){ \
		LOG("%s: failed enabling hook for %s, %d\n", __func__, STR(name), enable_status); \
		exit(1); \
	} \
	LOG("%s: hooked %s\n", __func__, STR(name)); \
}

#define HOOK_API(mod_name, name) { \
	void *target; \
	int create_status = MH_CreateHookApiEx(mod_name, STR(name), name##_hooked, (void **)&name##_orig, &target); \
	if (create_status != MH_OK){ \
		LOG("%s: failed creating hook for %ls %s, %d\n", __func__, mod_name, STR(name), create_status); \
		exit(1); \
	} \
	\
	int enable_status = MH_EnableHook(target); \
	if (enable_status != MH_OK){ \
		LOG("%s: failed enabling hook for %ls %s, %d\n", __func__, mod_name, STR(name), enable_status); \
		exit(1); \
	} \
	LOG("%s: hooked %ls %s\n", __func__, mod_name, STR(name)); \
}

#define HOOK_API2(mod_name, name) { \
	HMODULE lib_handle = LoadLibraryW(mod_name); \
	if (lib_handle == NULL){ \
		LOG("%s: failed loading library %ls, 0x%x\n", __func__, mod_name, GetLastError()); \
		exit(1); \
	} \
	void *target = (void *)GetProcAddress(lib_handle, STR(name)); \
	if (target == NULL){ \
		LOG("%s: failed loading %s from %ls\n", __func__, STR(name), mod_name); \
		exit(1); \
	} \
	int create_status = MH_CreateHook(target, (void *)name##_hooked, (void **)&name##_orig); \
	if (create_status != MH_OK){ \
		LOG("%s: failed creating hook for %ls %s, %d\n", __func__, mod_name, STR(name), create_status); \
		exit(1); \
	} \
	\
	int enable_status = MH_EnableHook(target); \
	if (enable_status != MH_OK){ \
		LOG("%s: failed enabling hook for %ls %s, %d\n", __func__, mod_name, STR(name), enable_status); \
		exit(1); \
	} \
	LOG("%s: hooked %ls %s\n", __func__, mod_name, STR(name)); \
}

std::map<void *, void *> xrGetVulkanDeviceExtensionsKHR_map;

// based on the findings of https://github.com/GloriousEggroll/proton-ge-custom/commit/a5a79f5b9d46fb3e02823d8653e91dd12496d3c7
int WINAPI xrGetVulkanDeviceExtensionsKHR_hooked(void *instance, void *systemId, uint32_t bufferCapacityInput, uint32_t *bufferCountOutput, char *buffer){
	auto map_entry = xrGetVulkanDeviceExtensionsKHR_map.find(instance);
	if (map_entry == xrGetVulkanDeviceExtensionsKHR_map.end()){
		LOG("%s: xrGetVulkanDeviceExtensionsKHR for instance %p was never registered\n", __func__, instance);
		exit(1);
	}
	int WINAPI (*xrGetVulkanDeviceExtensionsKHR_orig)(void *instance, void *systemId, uint32_t bufferCapacityInput, uint32_t *bufferCountOutput, char *buffer) = (int WINAPI (*)(void *instance, void *systemId, uint32_t bufferCapacityInput, uint32_t *bufferCountOutput, char *buffer))map_entry->second;
	int result = xrGetVulkanDeviceExtensionsKHR_orig(instance, systemId, bufferCapacityInput, bufferCountOutput, buffer);
	if (result == 0){
		LOG("%s: xrGetVulkanDeviceExtensionsKHR returning no feature\n", __func__);
		*bufferCountOutput = 0;
	}
	return result;
}


static int WINAPI (*xrGetInstanceProcAddr_orig)(void *instance, const char *fn_name, void **out_fn) = NULL;
int WINAPI xrGetInstanceProcAddr_hooked(void *instance, const char *fn_name, void **out_fn){
	int result = xrGetInstanceProcAddr_orig(instance, fn_name, out_fn);
	if (result == 0 && strcmp(fn_name, "xrGetVulkanDeviceExtensionsKHR") == 0){
		LOG("%s: redirected xrGetInstanceProcAddr_orig for instance %p\n", __func__, instance);
		xrGetVulkanDeviceExtensionsKHR_map[instance] = *out_fn;
		*out_fn = (void *)xrGetVulkanDeviceExtensionsKHR_hooked;
	}
	return result;
}

void hook_functions(){
	HOOK_API2(L"openxr_loader.dll", xrGetInstanceProcAddr);
}

void init_minhook(){
	MH_STATUS minhook_init_status = MH_Initialize();
	if (minhook_init_status != MH_OK && minhook_init_status != MH_ERROR_ALREADY_INITIALIZED){
		LOG("%s: failed initializing minhook, %d\n", __func__, minhook_init_status);
		exit(1);
	}
}

__attribute__((constructor))
int init(){
	init_log();
	dxgi_fetch_prog();

	init_minhook();
	hook_functions();

	LOG("%s: ready\n", __func__);
	printf("%s: beamng_proton_vr_shim ready\n", __func__);

	return 0;
}
