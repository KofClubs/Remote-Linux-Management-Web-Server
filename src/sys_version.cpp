#include "sys_version.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "exit_code.h"

void GetVersion(const char *filePath) {
  FILE *pf = fopen("/proc/version", "r");
  if (pf == NULL)
    exit(VERSION_FILE_NOT_EXIST);
  char str[VERSION_LENGTH];
  memset(str, '\0', sizeof(str));
  fgets(str, sizeof(str) - 1, pf);
  fclose(pf);
  pf = NULL;
  for (int i = 0; i < VERSION_LENGTH; i++)
    if (str[i + 1] == '(') {
      str[i] = '\0';
      std::ofstream fout(filePath, std::ios::app | std::ios::out);
      if (!fout)
        exit(HTML_OPEN_ERROR);
      fout << "<p>Linux Version of this server:</p>\n";
      fout << "<p>" << str << "</p>\n";
      fout.close();
      return;
    }
  exit(VERSION_STRING_TOO_SHORT);
}
