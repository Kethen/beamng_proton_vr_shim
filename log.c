#include <stdio.h>
#include <stdarg.h>

#define LOG_FILE "./beamng_proton_vr_shim.log"

void init_log(){
	FILE *log_file = fopen(LOG_FILE, "wb");
	if(log_file != NULL){
		fclose(log_file);
	}
}

void LOG(const char *fmt, ...){
	FILE *log_file = fopen(LOG_FILE, "ab");
	if (log_file == NULL){
		printf("log file open failed\n");
		return;
	}

	va_list args;
	va_start(args, fmt);
	vfprintf(log_file, fmt, args);
	va_end(args);

	fclose(log_file);
}
