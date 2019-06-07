#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <optional>


namespace StateReportable
{
  // Represents one line of the report
  struct ReportLine
  {
    std::string typeName;
    std::string stateFrom;
    std::string stateTo;
    // TODO: why optional?
    std::optional<long long> durationMicroSeconds;
  };

  std::ostream &operator<<(std::ostream &stream, const StateReportable::ReportLine &reportLine);

  // Converts str to ReportLine.
  // Throws an exception if str is not consistent.
  ReportLine convertToReportLine(const std::string &str);
} // namespace StateReportable
