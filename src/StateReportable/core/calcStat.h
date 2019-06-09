#pragma once

#include "StatLine.h"
#include "ReportLine.h"

#include <map>
#include <ostream>


namespace StateReportable::core
{
  // Key is pair of states - from-state and to-state
  using TransitionsStat = std::map<std::pair<std::string, std::string>, StatLine>;
  // Key is type name
  using FullStat = std::map<std::string, TransitionsStat>;


  template<typename RawStat>
  FullStat calcStat(typename RawStat::const_iterator first, typename RawStat::const_iterator last)
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


  template<typename RawStat>
  FullStat calcStat(const RawStat &rawStat)
  {
    return calcStat<RawStat>(rawStat.cbegin(), rawStat.cend());
  }


  std::ostream &operator<<(std::ostream &stream, const FullStat &fullStat);
} // namespace StateReportable::core
