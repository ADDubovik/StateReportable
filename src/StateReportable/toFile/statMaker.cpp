#include "Reporter.h"

#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char *argv[])
{
  if ( argc != 2 )
  {
    std::cout << "Please specify raw statistics file name as the first command line argument." << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);
  while ( !file.eof() )
  {
    std::string str;
    std::getline(file, str);

    if ( auto reportLine = StateReportable::core::convertToReportLine(str) )
      std::cout << *reportLine << std::endl;
  }

  return 0;
}
