#include <gtest/gtest.h>

#include "core/Dispatcher.h"
#include "core/ReportLine.h"

#include <vector>
#include <algorithm>
#include <array>


using namespace StateReportable;

using Storage = std::vector<core::ReportLine>;
Storage globalStorage;


struct TestDestination
{
  void send(core::ReportLine &&line)
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


core::ReportLine getReportLineRandom()
{
  return {getStringRandom(), getStringRandom(), getStringRandom(), rand() & 0xff};
}


TEST(DispatcherTest, Test_01)
{
  Storage expected = {getReportLineRandom()};
  if ( auto strong = core::Dispatcher<core::ReportLine, TestDestination>::instanceWeak().lock() )
    strong->send(core::ReportLine(*expected.cbegin()));
  else
    EXPECT_TRUE(false);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));  
  EXPECT_EQ(expected, globalStorage);

  globalStorage.clear();
}


TEST(DispatcherTest, Test_02)
{
  const auto lines = 10'000;

  Storage expected;
  for ( auto i = 0; i < lines; ++i )
  {
    if ( expected.size() == expected.capacity() )
      expected.reserve(std::max(1u, expected.size() * 2));

    expected.emplace_back(getReportLineRandom());
  }

  if ( auto strong = core::Dispatcher<core::ReportLine, TestDestination>::instanceWeak().lock() )
    for ( auto const &elem : expected )
      strong->send(core::ReportLine(elem));
  else
    EXPECT_TRUE(false);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));  

  EXPECT_EQ(expected.size(), globalStorage.size());

  std::sort(expected.begin(), expected.end());
  std::sort(globalStorage.begin(), globalStorage.end());
  EXPECT_EQ(expected, globalStorage);

  globalStorage.clear();
}


void sendSome(Storage::const_iterator begin, Storage::const_iterator end)
{
  if ( auto strong = core::Dispatcher<core::ReportLine, TestDestination>::instanceWeak().lock() )
    for ( auto iter = begin; iter != end; ++iter )
    {
      strong->send(core::ReportLine(*iter));
      std::this_thread::yield();
    }
  else
    EXPECT_TRUE(false);
}


template<typename Duration_t>
void test(size_t reportBySingleThread, size_t reporterThreads, Duration_t sleep)
{
  Storage expected;
  for ( size_t i = 0u; i < reportBySingleThread * reporterThreads; ++i )
  {
    if ( expected.size() == expected.capacity() )
      expected.reserve(std::max(1u, expected.size() * 2));

    expected.emplace_back(getReportLineRandom());
  }      

  {
    std::vector<std::future<void>> reporters;
    reporters.reserve(reporterThreads);
    for ( size_t i = 0; i < reporterThreads; ++i )
      reporters.emplace_back(std::async(
          std::launch::async,
          sendSome,
          expected.cbegin() + i * reportBySingleThread,
          expected.cbegin() + (i + 1) * reportBySingleThread
      ));
  } // <--- waiting for all reporters to finish

  std::this_thread::sleep_for(sleep);

  EXPECT_EQ(expected.size(), globalStorage.size());

  std::sort(expected.begin(), expected.end());
  std::sort(globalStorage.begin(), globalStorage.end());
  EXPECT_EQ(expected, globalStorage);

  globalStorage.clear();
}


TEST(DispatcherTest, Test_03)
{
  test(2'500, 4, std::chrono::milliseconds(100));
}


TEST(DispatcherTest, HeavyLoadTest)
{
  srand(static_cast<unsigned int>(time(nullptr)));

  const auto repetitions = 100;

  for ( auto rep = 0; rep < repetitions; ++rep )
  {
    const size_t reportBySingleThread = 100 + 100 * (rand() & 0xf);
    const size_t reporterThreads = 1 + rand() & 0xf;

    test(reportBySingleThread, reporterThreads, std::chrono::milliseconds(200));
  }
}


#include "main.h"