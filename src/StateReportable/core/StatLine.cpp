#include "StatLine.h"
#include "split.h"


namespace StateReportable::core
{
  using namespace std::string_literals;

  StatLine::StatLine(Duration duration)
      : pcs(1), min(duration), max(duration), total(duration)
  {};

  StatLine::StatLine(uint64_t pcs_, Duration min_, Duration max_, Duration total_)
      : pcs(pcs_), min(min_), max(max_), total(total_)
  {};

  void StatLine::add(Duration duration)
  {
    ++pcs;
    min = std::min(min, duration);
    max = std::max(max, duration);
    total += duration;
  }

  std::ostream &operator<<(std::ostream &stream, const StatLine &line)
  {
    stream << std::setw(8) << line.pcs
           << std::setw(12) << line.min
           << std::setw(12) << line.max;
    if ( line.pcs )
      stream << std::setw(12) << static_cast<long long>(static_cast<double>(line.total) / line.pcs);

    return stream;
  }
} // namespace StateReportable::core
