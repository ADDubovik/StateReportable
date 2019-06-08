#pragma once

#include <ostream>
#include <string>


namespace StateReportable::core
{
  // Represents one line of the report
  struct ReportLine
  {
    std::string typeName;
    std::string stateFrom;
    std::string stateTo;
    long long duration;

    ReportLine() = delete;

    // Perfect forwarding for strings.
    // User should provide all the data in c-tor to avoid non-initialized fields.
    template<typename T1, typename T2, typename T3>
    ReportLine(T1 &&typeName_, T2 &&stateFrom_, T3 &&stateTo_, long long duration_)
        : typeName(std::forward<T1>(typeName_)),
        stateFrom(std::forward<T2>(stateFrom_)),
        stateTo(std::forward<T3>(stateTo_)),
        duration(duration_)
    {}
    ~ReportLine() = default;

    ReportLine(const ReportLine &) = default;
    ReportLine(ReportLine &&) = default;

    ReportLine& operator=(const ReportLine &) = default;
    ReportLine& operator=(ReportLine &&) = default;

  private:
    auto rank() const { return std::tie(typeName, stateFrom, stateTo, duration); };

  public:
    bool operator==(const ReportLine &oth) const { return rank() == oth.rank(); };
    bool operator!=(const ReportLine &oth) const { return rank() != oth.rank(); };
    bool operator<(const ReportLine &oth)  const { return rank() <  oth.rank(); };
    bool operator>(const ReportLine &oth)  const { return rank() >  oth.rank(); };
  };

  std::ostream &operator<<(std::ostream &stream, const ReportLine &reportLine);

  // Converts str to ReportLine.
  // Throws an exception if str is not consistent.
  ReportLine convertToReportLine(const std::string &str);
} // namespace StateReportable::core
