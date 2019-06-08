#include <gtest/gtest.h>

#include "core/Dispatcher.h"
#include "core/DispatcherImplementation.h"

#include <memory>
#include <functional>
#include <iostream>


// Tests just one core::Dispatcher mechanism: weak_singletone.
// Separate application needed for such type of test.


using namespace StateReportable;


struct TestDestination
{
  using Data = double;

  void send(Data &&line) {}
};


namespace
{
  void instantiateTemplateFunctions()
  {
    core::Dispatcher<TestDestination>::instanceWeak();
  }
} // namespace


using ScopeGuard = std::unique_ptr<int, std::function<void(int *)>>;
ScopeGuard longLiveGuard;


TEST(DispatcherWeakSingletonTest, Test_01)
{
  int i = 0;
  // Construction order:
  // - longLiveGuard;
  // - singletone;
  // - guard.
  // So destruction order should be:
  // - guard;
  // - singletone;
  // - longLiveGuard
  ScopeGuard guard(
      &i,
      [weak = core::Dispatcher<TestDestination>::instanceWeak()](int *)
      {
        std::cout << "OK" << std::endl;
        EXPECT_FALSE(weak.lock());
      }
  );
  // To make singletone to be destroyed before guard.
  longLiveGuard = std::move(guard);
}


#include "main.h"