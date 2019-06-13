#include "Reporter.h"
#include "ConfigReader.h"
#include "StateReportable/core/DispatcherImplementation.h"

#include <filesystem>


namespace StateReportable::toFile
{
  const std::filesystem::path configPath("StateReportable.config");

  Reporter::Reporter()
  {
    if ( auto name = readFilenameFromFile(configPath) )
      m_file = std::ofstream(*name, std::ios::app);
  }

  void Reporter::send(Data && line)
  {
    if ( m_file )
      *m_file << line << '\n';
  }
} // namespace StateReportable::toFile


namespace
{
  void instantiateTemplateFunctions()
  {
    StateReportable::core::Dispatcher<StateReportable::toFile::Reporter>::instanceWeak();
  }
} // namespace
