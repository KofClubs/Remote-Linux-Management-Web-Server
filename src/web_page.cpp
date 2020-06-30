#include "web_page.h"

#include <fstream>

#include "exit_code.h"

void OpenHTMLFile(const char *filePath) {
  std::ofstream fout(filePath, std::ios::out);
  if (!fout)
    exit(HTML_OPEN_ERROR);
  fout << "<html>\n";
  fout << "<head>\n";
  fout << "<title>\n";
  fout << "System Information\n";
  fout << "</title>\n";
  fout << "</head>\n";
  fout << "<body>\n";
  fout.close();
}

void WriteHTMLFile(const char *filePath, void (*func)(const char *)) {
  func(filePath);
}

void CloseHTMLFile(const char *filePath) {
  std::ofstream fout(filePath, std::ios::app | std::ios::out);
  if (!fout)
    exit(HTML_OPEN_ERROR);
  fout << "</body>\n";
  fout << "</html>\n";
  fout.close();
}