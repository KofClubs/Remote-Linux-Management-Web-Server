#include "sys_memfree.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "exit_code.h"

void GetMemFree(const char *filePath) {
  FILE *pf = fopen("/proc/meminfo", "r");
  if (pf == NULL)
    exit(MEMINFO_FILE_NOT_EXIST);
  char str[MEMFREE_LENGTH];
  while (!feof(pf)) {
    memset(str, '\0', sizeof(str));
    fgets(str, sizeof(str) - 1, pf);
    if (str[0] == 'M' && str[3] == 'F') {
      fclose(pf);
      pf = NULL;
      std::ofstream fout(filePath, std::ios::app | std::ios::out);
      if (!fout)
        exit(HTML_OPEN_ERROR);
      fout << "<p>Free memory of this server:<\p>\n";
      fout << "<p>" << str << "<\p>\n";
      fout.close();
      return;
    }
  }
  fclose(pf);
  pf = NULL;
  exit(MEMINFO_STRING_TOO_SHORT);
}
