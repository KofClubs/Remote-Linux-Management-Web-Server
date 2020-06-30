#include "sys_time.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "exit_code.h"

void GetTime(const char *filePath) {
  time_t t;
  time(&t);
  std::ofstream fout(filePath, std::ios::app | std::ios::out);
  if (!fout)
    exit(HTML_OPEN_ERROR);
  fout << "<p>Local time of this server:<\p>\n";
  fout << "<p>" << ctime(&t) << "<\p>\n";
  fout.close();
  printf("%s", ctime(&t));
}