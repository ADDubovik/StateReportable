#pragma once

#include <string>
#include <vector>


namespace StateReportable::core
{
  // something like boost::algorithm::split
  // Splits str into words separated by separator
  std::vector<std::string> split(const std::string &str, char separator);
} // namespace StateReportable::core
