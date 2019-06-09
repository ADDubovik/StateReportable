#include "Reporter.h"
#include "core/VectorHelpers.h"
#include "core/calcStat.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main(int argc, char *argv[])
{
  if ( argc != 2 )
  {
    std::cout << "Please specify raw statistics file name as the first command line argument." << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  std::vector<StateReportable::core::ReportLine> rawStat;
  while ( !file.eof() )
  {
    std::string str;
    std::getline(file, str);

    if ( auto reportLine = StateReportable::core::convertToReportLine(str) )
    {
      VectorHelpers::doubleCapacityIfNeeded(rawStat);
      rawStat.emplace_back(*reportLine);
    }
  }

  auto stat = StateReportable::core::calcStat(rawStat);

  std::cout << stat;

  return 0;
}
