#include "ReportLine.h"
#include "split.h"


namespace StateReportable
{
  const char separator = '\t';

  ReportLine convertToReportLine(const std::string &str)
  {
    auto splitted = split(str, separator);

    if ( splitted.size() != 4u )
      throw std::runtime_error(std::string("Incorrect string in ") + __FUNCTION__);

    return ReportLine(std::move(splitted[0]),
                      std::move(splitted[1]),
                      std::move(splitted[2]),
                      atoll(splitted[3].c_str()));
  }

  std::ostream &operator<<(std::ostream &stream, const StateReportable::ReportLine &reportLine)
  {
    stream << reportLine.typeName << StateReportable::separator
           << reportLine.stateFrom << StateReportable::separator
           << reportLine.stateTo << StateReportable::separator
           << reportLine.duration;

    return stream;
  }
} // namespace StateReportable
