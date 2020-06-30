#ifndef __SYS_PROCESS_H__
#define __SYS_PROCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PROCESS_NUMBER 1000
#define PROCESS_LENGTH (1 << 7)

void GetProcesses(const char *);

#ifdef __cplusplus
}
#endif

#endif
