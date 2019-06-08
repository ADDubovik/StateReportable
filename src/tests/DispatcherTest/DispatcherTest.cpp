#include <gtest/gtest.h>

#include "Dispatcher.h"
#include "ReportLine.h"

#include <vector>
#include <algorithm>
#include <array>


using namespace StateReportable;

using Storage = std::vector<ReportLine>;
Storage globalStorage;


struct TestDestination
{
  void send(ReportLine &&line)
  {
    if ( globalStorage.size() == globalStorage.capacity() )
      globalStorage.reserve(std::max(1u, globalStorage.size() * 2));

    globalStorage.emplace_back(std::move(line));
  }
};


std::string getStringRandom()
{
  static std::array<unsigned char, 16> alphabet =
      {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
  auto chars = 1 + (rand() & 0xf);
  std::string result;
  for ( auto i = 0; i < chars; ++i )
    result += alphabet[rand() & 0xf];

  return result;
}


ReportLine getReportLineRandom()
{
  return {getStringRandom(), getStringRandom(), getStringRandom(), rand() & 0xff};
}


TEST(DispatcherTest, Test_01)
{
  Storage expected = {getReportLineRandom()};
  if ( auto lock = Dispatcher<ReportLine, TestDestination>::instanceWeak().lock() )
    lock->send(ReportLine(*expected.cbegin()));
  else
    EXPECT_TRUE(false);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));  
  EXPECT_EQ(expected, globalStorage);
}


#include "main.h"