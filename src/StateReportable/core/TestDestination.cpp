#include "TestDestination.h"

#include <algorithm>


namespace StateReportable::core
{
  void TestDestination::send(Data &&line)
  {
    std::lock_guard<std::mutex> guard(storageWithMutex.first);
    if ( storageWithMutex.second.size() == storageWithMutex.second.capacity() )
      storageWithMutex.second.reserve(std::max(1u, storageWithMutex.second.size() * 2));

    storageWithMutex.second.emplace_back(std::move(line));
  }
} // namespace StateReportable::core