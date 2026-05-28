#ifndef __LOG_H
#define __LOG_H

#ifdef __cplusplus
extern "C" {
#endif

void init_log();
void LOG(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
