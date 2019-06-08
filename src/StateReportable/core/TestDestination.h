#pragma once

#include "ReportLine.h"

#include <vector>
#include <mutex>


namespace StateReportable::core
{
  // To test Dispatcher only
  struct TestDestination
  {
    using Data = core::ReportLine;
    using Storage = std::vector<Data>;
    using StorageWithMutex = std::pair<std::mutex, Storage>;

    StorageWithMutex storageWithMutex;

    void send(Data &&line);
  };
} // namespace StateReportable::core
