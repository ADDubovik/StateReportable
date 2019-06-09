#include "calcStat.h"

#include <iomanip>


namespace StateReportable::core
{
  std::ostream &operator<<(std::ostream &stream, const FullStat &fullStat)
  {
    size_t outerCounter = 1;
    for ( auto outer = fullStat.cbegin(); outer != fullStat.cend(); ++outer, ++outerCounter )
    {
      stream << outer->first << "\n";
      stream << std::setw(16) << "From state"
              << std::setw(16) << "To state"
              << std::setw(8) << "Pcs"
              << std::setw(12) << "Min"
              << std::setw(12) << "Max"
              << std::setw(12) << "Average" << "\n";
      size_t innerCounter = 1;
      for ( auto inner = outer->second.cbegin(); inner != outer->second.cend(); ++inner, ++innerCounter )
      {
        stream << std::setw(16) << inner->first.first
                << std::setw(16) << inner->first.second
                << inner->second;

        // To avoid unnecessary line feed at the end of processing
        if ( outerCounter != fullStat.size() || innerCounter != outer->second.size() )
          stream << "\n";
      }
    }

    return stream;
  }
} // namespace StateReportable::core
