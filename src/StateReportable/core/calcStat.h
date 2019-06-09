#pragma once

#include "StatLine.h"
#include "ReportLine.h"

#include <map>
#include <vector>
#include <ostream>


namespace StateReportable::core
{
  // Key is pair of states - from-state and to-state
  using TransitionsStat = std::map<std::pair<std::string, std::string>, StatLine>;
  // Key is type name
  using FullStat = std::map<std::string, TransitionsStat>;

  using RawStat = std::vector<ReportLine>;

  FullStat calcStat(const RawStat &rawStat);

  FullStat calcStat(RawStat::const_iterator first, RawStat::const_iterator last);


  std::ostream &operator<<(std::ostream &stream, const FullStat &fullStat);
} // namespace StateReportable::core
