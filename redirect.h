#ifndef __REDIRECT_H
#define __REDIRECT_H

#include <sysinfoapi.h>

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

#define REDIRECT_STR(s) #s

#define REDIRECT(name, ret, args, call) \
	static ret (*name##_orig) args = NULL; \
	ret name args { return name##_orig call; }

#define FETCH_REDIRECT_WIN(mod_name, name) { \
	char win_dir[1024] = {0}; \
	GetWindowsDirectoryA(win_dir, sizeof(win_dir) - 1); \
	char lib_path[1024] = {0}; \
	sprintf(lib_path, "%s/system32/%s", win_dir, mod_name); \
	HANDLE mod_handle = LoadLibraryA(lib_path); \
	if (mod_handle == NULL){ \
		LOG("%s: failed loading %s\n", __func__, lib_path); \
		sprintf(lib_path, "%s/syswow64/%s", win_dir, mod_name); \
		mod_handle = LoadLibraryA(lib_path); \
	} \
	if (mod_handle == NULL){ \
		LOG("%s: failed loading %s\n", __func__, lib_path); \
		exit(1); \
	} \
	name##_orig = (void *)GetProcAddress(mod_handle, REDIRECT_STR(name)); \
	if (name##_orig == NULL){ \
		LOG("%s: failed fetching %s from %s\n", __func__, REDIRECT_STR(name), mod_name); \
		exit(1); \
	} \
	LOG("%s: %s %s redirection ready\n", __func__, mod_name, REDIRECT_STR(name)); \
}

#endif
