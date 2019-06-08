#include "ReporterToFile.h"
#include "StateToFileConfigReader.h"
#include "core/DispatcherImplementation.h"

#include <filesystem>


namespace StateReportable
{
  const std::filesystem::path configPath("StateReportable.config");

  ReporterToFile::ReporterToFile()
  {
    if ( auto name = readFilenameFromFile(configPath) )
      m_file = std::ofstream(*name);
  }

  void ReporterToFile::send(Data && line)
  {
    if ( m_file )
      *m_file << line << '\n';
  }
} // namespace StateReportable


namespace
{
  void instantiateTemplateFunctions()
  {
    StateReportable::core::Dispatcher<StateReportable::ReporterToFile>::instanceWeak();
  }
} // namespace
