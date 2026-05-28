#include <stdio.h>
#include <stdarg.h>

#include <mutex>

#define LOG_FILE "./beamng_proton_vr_shim.log"

static std::mutex log_mutex;

extern "C" {

void init_log(){
	log_mutex.lock();
	FILE *log_file = fopen(LOG_FILE, "wb");
	if(log_file != NULL){
		fclose(log_file);
	}
	log_mutex.unlock();
}

void LOG(const char *fmt, ...){
	log_mutex.lock();
	FILE *log_file = fopen(LOG_FILE, "ab");
	if (log_file == NULL){
		printf("log file open failed\n");
		log_mutex.unlock();
		return;
	}

	va_list args;
	va_start(args, fmt);
	vfprintf(log_file, fmt, args);
	va_end(args);

	fclose(log_file);
	log_mutex.unlock();
}

}
