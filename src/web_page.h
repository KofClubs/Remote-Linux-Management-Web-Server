#ifndef __WEB_PAGE_H__
#define __WEB_PAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

void OpenHTMLFile(const char *);
void WriteHTMLFile(const char *, void (*)(const char *));
void CloseHTMLFile(const char *);

#ifdef __cplusplus
}
#endif

#endif