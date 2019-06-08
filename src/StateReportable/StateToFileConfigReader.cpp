#include "StateToFileConfigReader.h"

#include <fstream>


namespace StateReportable
{
  std::optional<std::filesystem::path> readFilenameFromFile(const std::filesystem::path &configFileName)
  {
    std::ifstream file(configFileName);
    std::filesystem::path result;
    file >> result;

    if (file.fail())
    {
      // TODO: add project-specific logging here
      return std::nullopt;
    }

    // TODO: add project-specific logging here
    return result;
  }
} // namespace StateReportable
