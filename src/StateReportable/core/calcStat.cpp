#include "calcStat.h"

#include <iomanip>


namespace StateReportable::core
{
  FullStat calcStat(RawStat::const_iterator first, RawStat::const_iterator last)
  {
    FullStat result;

    for ( auto iter = first; iter != last; ++iter )
    {
      auto outer = result.try_emplace(iter->typeName, FullStat::mapped_type());
      auto inner = outer.first->second.try_emplace(
          TransitionsStat::key_type(iter->stateFrom, iter->stateTo),
          StatLine(iter->duration));

      // If no insertion happened  ...
      if ( !inner.second )
        // ... current value need to be added to the stat line 
        inner.first->second.add(iter->duration);
    }

    return result;
  }

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
        if ( outerCounter != outer->second.size() || innerCounter != outer->second.size() )
          stream << "\n";
      }
    }

    return stream;
  }
} // namespace StateReportable::core
