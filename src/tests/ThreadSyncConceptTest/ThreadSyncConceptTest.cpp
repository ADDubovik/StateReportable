#include <gtest/gtest.h>


// Tests a concept of synchonizing messages transfer from a lot of reporter threads
// to a signle collector thread


#include <atomic>
#include <future>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>


using Elem = size_t;
using Storage = std::vector<size_t>;
using Exchanger = std::pair<std::mutex, Storage>;


size_t collect(Exchanger &exchanger, std::atomic<bool> &stopCollecting)
{
  // To count all unique elements
  std::set<Elem> localStorage;

  auto grabExchangerToLocal = [&localStorage](auto &exch)
  {
    std::lock_guard<std::mutex> guard(exch.first);
    for ( auto &&elem : exch.second )
      localStorage.emplace(std::move(elem));
    exch.second.clear();
  };

  while ( !stopCollecting )
  {
    grabExchangerToLocal(exchanger);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  // ... and once more after stop
  grabExchangerToLocal(exchanger);

  return localStorage.size();
}


void report(size_t startFrom, size_t howMuch, Exchanger &exchanger)
{
  for ( size_t i = 0; i < howMuch; ++i )
  {
    {
      std::lock_guard<std::mutex> guard(exchanger.first);
      if ( exchanger.second.size() == exchanger.second.capacity() )
        exchanger.second.reserve(std::max<size_t>(1u, exchanger.second.size() * 2));

      exchanger.second.emplace_back(startFrom + i);
    }

    std::this_thread::yield();
  }
}


void test(size_t reportBySingleThread, size_t reporterThreads)
{
  Exchanger exchanger;
  std::atomic<bool> stopCollecting = false;

  auto collector = std::async(std::launch::async, collect, std::ref(exchanger), std::ref(stopCollecting));

  {
    std::vector<std::future<void>> reporters;
    reporters.reserve(reporterThreads);
    for ( size_t i = 0; i < reporterThreads; ++i )
      reporters.emplace_back(std::async(
          std::launch::async,
          report,
          i * reportBySingleThread,
          reportBySingleThread,
          std::ref(exchanger)
      ));
  } // <--- waiting for all reporters to finish

  stopCollecting = true;

  EXPECT_EQ(collector.get(), reportBySingleThread * reporterThreads);
}


TEST(ThreadSyncConceptTest, Single_repetition)
{
  test(25'000, 4);
}


TEST(ThreadSyncConceptTest, GreatRandomTest)
{
  srand(static_cast<unsigned int>(time(nullptr)));

  const auto repetitions = 100;

  for ( auto rep = 0; rep < repetitions; ++rep )
  {
    const size_t reportBySingleThread = 1'000 + 1'000 * (rand() & 0xf);
    const size_t reporterThreads = 1 + rand() & 0xf;

    test(reportBySingleThread, reporterThreads);
  }
}


#include "main.h"