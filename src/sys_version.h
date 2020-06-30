#ifndef __SYS_VERSION_H__
#define __SYS_VERSION_H__

#ifdef __cplusplus
extern "C" {
#endif

#define VERSION_LENGTH (1 << 8)

void GetVersion(const char *);

#ifdef __cplusplus
}
#endif

#endif
