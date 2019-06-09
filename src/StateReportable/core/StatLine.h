#pragma once

#include <cstdint>
#include <type_traits>
#include <ostream>
#include <algorithm>


namespace StateReportable::core
{
  // Represents one line of statistics
  template<typename T = long long>
  struct StatLine
  {
    static_assert(std::is_arithmetic_v<T>);

    uint64_t pcs;
    T min;
    T max;
    T total;

    StatLine() = delete;

    StatLine(T duration) : pcs(1), min(duration), max(duration), total(duration) {};
    // For use in tests
    StatLine(uint64_t pcs_, T min_, T max_, T total_) : pcs(pcs_), min(min_), max(max_), total(total_) {};
    ~StatLine() = default;

    StatLine(const StatLine &) = default;
    StatLine(StatLine &&) = default;

    StatLine &operator=(const StatLine &) = default;
    StatLine &operator=(StatLine &&) = default;

    void add(T duration)
    {
      ++pcs;
      min = std::min(min, duration);
      max = std::max(max, duration);
      total += duration;
    }

  private:
    auto rank() const { return std::tie(pcs, min, max, total); }

  public:
    bool operator==(const StatLine &oth) const { return rank() == oth.rank(); };
    bool operator!=(const StatLine &oth) const { return rank() != oth.rank(); };
  };

  template<typename T>
  std::ostream &operator<<(std::ostream &stream, const StatLine<T> &line)
  {
    stream << std::setw(8) << line.pcs
           << std::setw(12) << line.min
           << std::setw(12) << line.max;
    if ( line.pcs )
      stream << std::setw(12) << static_cast<long long>(static_cast<double>(line.total) / line.pcs);

    return stream;
  }
} // namespace StateReportable::core
