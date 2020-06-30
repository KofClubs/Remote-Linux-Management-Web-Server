#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TIME_LENGTH (1 << 5)

void GetTime(const char *);

#ifdef __cplusplus
}
#endif

#endif