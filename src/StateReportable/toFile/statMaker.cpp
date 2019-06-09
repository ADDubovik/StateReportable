#include "Reporter.h"
#include "core/VectorHelpers.h"
#include "core/calcStat.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <atomic>


// Ctrl-C flag
static std::atomic<bool> interrupted = false;


#ifdef _WIN32
#include <windows.h>
BOOL WINAPI consoleHandler(DWORD)
{
  interrupted = true;
  return TRUE;
}
#else
#include <csignal>
void consoleHandler(int s)
{
  interrupted = true;
}
#endif


int main(int argc, char *argv[])
{
#ifdef _WIN32
  // For Ctrl-C processing
  SetConsoleCtrlHandler(consoleHandler, TRUE);
#else
  struct sigaction sigIntHandler { {consoleHandler}};
  sigemptyset(&sigIntHandler.sa_mask);
  sigaction(SIGINT, &sigIntHandler, nullptr);
#endif

  if ( argc != 2 )
  {
    std::cout << "Please specify raw statistics file name as the first command line argument." << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector<StateReportable::core::ReportLine> rawStat;
  while ( !file.eof() && !interrupted )
  {
    std::string str;
    std::getline(file, str);

    if ( auto reportLine = StateReportable::core::convertToReportLine(str) )
    {
      VectorHelpers::doubleCapacityIfNeeded(rawStat);
      rawStat.emplace_back(*reportLine);
    }
  }

  if ( !interrupted )
  {
    auto stat = StateReportable::core::calcStat(rawStat);

    std::cout << stat;
  }

  return 0;
}
