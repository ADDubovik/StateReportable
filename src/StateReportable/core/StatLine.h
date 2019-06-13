#pragma once

#include "ReportLine.h"

#include <cstdint>
#include <type_traits>
#include <ostream>
#include <algorithm>
#include <iomanip>
#include <tuple>


namespace StateReportable::core
{
  // Represents one line of statistics
  struct StatLine
  {
    using Duration = typename ReportLine::Duration;

    uint64_t pcs;
    Duration min;
    Duration max;
    Duration total;

    StatLine() = delete;

    StatLine(Duration duration);
    // For use in tests
    StatLine(uint64_t pcs_, Duration min_, Duration max_, Duration total_);
    ~StatLine() = default;

    StatLine(const StatLine &) = default;
    StatLine(StatLine &&) = default;

    StatLine &operator=(const StatLine &) = default;
    StatLine &operator=(StatLine &&) = default;

    void add(Duration duration);

  private:
    auto rank() const { return std::tie(pcs, min, max, total); }

  public:
    bool operator==(const StatLine &oth) const { return rank() == oth.rank(); };
    bool operator!=(const StatLine &oth) const { return rank() != oth.rank(); };
  };

  std::ostream &operator<<(std::ostream &stream, const StatLine &line);
} // namespace StateReportable::core
