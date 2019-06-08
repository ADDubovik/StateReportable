#include <gtest/gtest.h>

#include "core/Dispatcher.h"
#include "core/ReportLine.h"

#include <vector>
#include <algorithm>
#include <array>


using namespace StateReportable;

using Storage = std::vector<core::ReportLine>;
using StorageWithMutex = std::pair<std::mutex, Storage>;
StorageWithMutex globalStorage;


struct TestDestination
{
  using Data = core::ReportLine;

  void send(Data &&line)
  {
    std::lock_guard<std::mutex> guard(globalStorage.first);
    if ( globalStorage.second.size() == globalStorage.second.capacity() )
      globalStorage.second.reserve(std::max(1u, globalStorage.second.size() * 2));

    globalStorage.second.emplace_back(std::move(line));
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
  if ( auto strong = core::Dispatcher<TestDestination>::instanceWeak().lock() )
    strong->send(core::ReportLine(*expected.cbegin()));
  else
    EXPECT_TRUE(false);

  while (true)
  {
    {
      std::lock_guard<std::mutex> guard(globalStorage.first);
      if ( globalStorage.second.size() == expected.size() )
        break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::lock_guard<std::mutex> guard(globalStorage.first);
  EXPECT_EQ(expected, globalStorage.second);

  globalStorage.second.clear();
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

  if ( auto strong = core::Dispatcher<TestDestination>::instanceWeak().lock() )
    for ( auto const &elem : expected )
      strong->send(core::ReportLine(elem));
  else
    EXPECT_TRUE(false);

  while (true)
  {
    {
      std::lock_guard<std::mutex> guard(globalStorage.first);
      if ( globalStorage.second.size() == expected.size() )
        break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::lock_guard<std::mutex> guard(globalStorage.first);
  EXPECT_EQ(expected.size(), globalStorage.second.size());

  std::sort(expected.begin(), expected.end());
  std::sort(globalStorage.second.begin(), globalStorage.second.end());
  EXPECT_EQ(expected, globalStorage.second);

  globalStorage.second.clear();
}


void sendSome(Storage::const_iterator begin, Storage::const_iterator end)
{
  if ( auto strong = core::Dispatcher<TestDestination>::instanceWeak().lock() )
    for ( auto iter = begin; iter != end; ++iter )
    {
      strong->send(core::ReportLine(*iter));
      std::this_thread::yield();
    }
  else
    EXPECT_TRUE(false);
}


void test(size_t reportBySingleThread, size_t reporterThreads)
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

  while (true)
  {
    {
      std::lock_guard<std::mutex> guard(globalStorage.first);
      if ( globalStorage.second.size() == expected.size() )
        break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::lock_guard<std::mutex> guard(globalStorage.first);
  EXPECT_EQ(expected.size(), globalStorage.second.size());

  std::sort(expected.begin(), expected.end());
  std::sort(globalStorage.second.begin(), globalStorage.second.end());
  EXPECT_EQ(expected, globalStorage.second);

  globalStorage.second.clear();
}


TEST(DispatcherTest, Test_03)
{
  test(2'500, 4);
}


TEST(DispatcherTest, HeavyLoadTest)
{
  srand(static_cast<unsigned int>(time(nullptr)));

  const auto repetitions = 100;

  for ( auto rep = 0; rep < repetitions; ++rep )
  {
    const size_t reportBySingleThread = 100 + 100 * (rand() & 0xf);
    const size_t reporterThreads = 1 + rand() & 0xf;

    test(reportBySingleThread, reporterThreads);
  }
}


#include "main.h"