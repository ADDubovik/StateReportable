#include "ReportLine.h"
#include "split.h"


namespace StateReportable::core
{
  const char separator = '\t';

  std::optional<ReportLine> convertToReportLine(const std::string &str)
  {
    auto splitted = split(str, separator);

    if ( splitted.size() != 4u )
      return std::nullopt;

    return ReportLine(std::move(splitted[0]),
                      std::move(splitted[1]),
                      std::move(splitted[2]),
                      atoll(splitted[3].c_str()));
  }

  std::ostream &operator<<(std::ostream &stream, const ReportLine &reportLine)
  {
    stream << reportLine.typeName << separator
           << reportLine.stateFrom << separator
           << reportLine.stateTo << separator
           << reportLine.duration;

    return stream;
  }
} // namespace StateReportable::core
