#include <gtest/gtest.h>

#include "Dispatcher.h"
#include "ReportLine.h"

#include <vector>
#include <algorithm>


using namespace StateReportable;

struct TestDestination
{
  void send(ReportLine &&line)
  {
    if ( storage.size() == storage.capacity() )
      storage.reserve(std::max(1u, storage.size() * 2));

    storage.emplace_back(std::move(line));
  }

  using Storage = std::vector<ReportLine>;
  Storage storage;
};


TEST(DispatcherTest, Test_01)
{
  Dispatcher<ReportLine, TestDestination>::instanceWeak();
}


#include "main.h"