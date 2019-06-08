#include "ReporterToFile.h"
#include "StateToFileConfigReader.h"

#include <filesystem>



namespace StateReportable
{
  const std::filesystem::path configPath("StateReportable.config");

  ReporterToFile::ReporterToFile()
  {
    if ( auto name = readFilenameFromFile(configPath) )
      m_file = std::ofstream(*name);
  }

  void ReporterToFile::send(core::ReportLine && line)
  {
    if ( m_file )
      *m_file << line;
  }
} // namespace StateReportable
