#ifndef __SYS_MEMFREE_H__
#define __SYS_MEMFREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MEMFREE_LENGTH (1 << 5)

void GetMemFree(const char *);

#ifdef __cplusplus
}
#endif

#endif
