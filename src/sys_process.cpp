#include "sys_process.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "exit_code.h"

void GetProcesses(const char *filePath) {
  system("top -b -n1 > /tmp/process");
  FILE *pf = fopen("/tmp/process", "r");
  if (pf == NULL)
    exit(CREATE_PROCESS_FILE_FAILURE);
  char str[PROCESS_NUMBER][PROCESS_LENGTH];
  int tmp = 0;
  while (!feof(pf)) {
    memset(str[tmp], '\0', sizeof(str[tmp]));
    fgets(str[tmp], sizeof(str[tmp]) - 1, pf);
    tmp++;
  }
  fclose(pf);
  pf = NULL;
  system("rm -f /tmp/process");
  std::ofstream fout(filePath, std::ios::app | std::ios::out);
  if (!fout)
    exit(HTML_OPEN_ERROR);
  fout << "<p>Processes of this server:<\p>\n";
  for (int i = 0; i < tmp; i++)
    fout << "<p>" << str[i] << "<\p>\n";
  fout.close();
}
