#include "ReportLine.h"

#include <regex>


namespace StateReportable
{
  const char separator = '\t';

  ReportLine convertToReportLine(const std::string &str)
  {
    std::regex regex(std::string("[^") + separator + "]+");
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), regex);
    auto words_end = std::sregex_iterator();
    size_t i = 0;
    ReportLine result;
    for (std::sregex_iterator iter = words_begin; iter != words_end; ++iter, ++i)
    {
      switch(i)
      {
        case 0:
          result.typeName = iter->str();
          break;
        case 1:
          result.stateFrom = iter->str();
          break;
        case 2:
          result.stateTo = iter->str();
          break;
        case 3:
          result.durationMicroSeconds = atoll(iter->str().c_str());
          break;
      }
    }

    if ( i != 4 )
      throw std::runtime_error(std::string("Incorrect string in ") + __FUNCTION__);

    return result;
  }

  std::ostream &operator<<(std::ostream &stream, const StateReportable::ReportLine &reportLine)
  {
    stream << reportLine.typeName << StateReportable::separator
           << reportLine.stateFrom << StateReportable::separator
           << reportLine.stateTo << StateReportable::separator;

    if (reportLine.durationMicroSeconds)
      stream << *reportLine.durationMicroSeconds;
    return stream;
  }
} // namespace StateReportable
