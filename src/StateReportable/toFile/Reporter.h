#pragma once

#include "core/ReportLine.h"

#include <fstream>
#include <optional>


namespace StateReportable::toFile
{
  class Reporter
  {
  public:
    using Data = core::ReportLine;

    Reporter();
    ~Reporter() = default;

    Reporter(const Reporter &) = delete;
    Reporter(Reporter &&) = delete;

    Reporter& operator=(const Reporter &) = delete;
    Reporter& operator=(Reporter &&) = delete;

    void send(Data &&line);

  private:
    std::optional<std::ofstream> m_file;
  };
} // namespace StateReportable::toFile
