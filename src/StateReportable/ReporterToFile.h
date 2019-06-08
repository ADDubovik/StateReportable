#pragma once

#include "core/ReportLine.h"

#include <fstream>
#include <optional>


namespace StateReportable
{
  class ReporterToFile
  {
  public:
    ReporterToFile();
    ~ReporterToFile() = default;

    ReporterToFile(const ReporterToFile &) = delete;
    ReporterToFile(ReporterToFile &&) = delete;

    ReporterToFile& operator=(const ReporterToFile &) = delete;
    ReporterToFile& operator=(ReporterToFile &&) = delete;

    void send(core::ReportLine &&line);

  private:
    std::optional<std::ofstream> m_file;
  };
} // namespace StateReportable
