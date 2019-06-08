#pragma once

#include <optional>
#include <filesystem>


namespace StateReportable::toFile
{
  // Reads a filename from configFileName file
  std::optional<std::filesystem::path> readFilenameFromFile(const std::filesystem::path &configFileName);
} // namespace StateReportable::toFile
