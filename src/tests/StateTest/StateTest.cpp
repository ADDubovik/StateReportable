#include <gtest/gtest.h>

#include "State.h"

#include <thread>


using namespace std::string_literals;

namespace Namespace_01
{
  enum class State
  {
    non_inintialized,
    connecting,
    connected,
    error
  };

  std::ostream &operator<<(std::ostream &stream, const State state)
  {
    switch (state)
    {
      case State::non_inintialized: return stream << "non_inintialized"; break;
      case State::connecting: return stream << "connecting"; break;
      case State::connected: return stream << "connected"; break;
      case State::error: return stream << "error"; break;
      default : return stream << "unknown";
    };
  }
} // namespace Namespace_01

namespace Namespace_02
{
  enum class State
  {
    none,
    preparing,
    working,
    finished,
    error
  };

  std::ostream &operator<<(std::ostream &stream, const State state)
  {
    switch (state)
    {
      case State::none: return stream << "none";
      case State::preparing: return stream << "preparing";
      case State::working: return stream << "working";
      case State::finished: return stream << "finished";
      case State::error: return stream << "error";
      default : return stream << "unknown";
    };
  }
} // namespace Namespace_02


using namespace StateReportable;


TEST(StateTest, Test_01)
{
  EXPECT_NO_THROW(std::cout
                  << Namespace_01::State::non_inintialized << " "
                  << Namespace_01::State::connecting << " "
                  << Namespace_01::State::connected << " "
                  << Namespace_01::State::error << "\n");
  EXPECT_NO_THROW(std::cout
                  << Namespace_02::State::none << " "
                  << Namespace_02::State::preparing << " "
                  << Namespace_02::State::working << " "
                  << Namespace_02::State::finished << " "
                  << Namespace_02::State::error << "\n");
}


TEST(StateTest, Test_02)
{
  std::stringstream stream;
  
  {
    State<Namespace_01::State, std::chrono::milliseconds>
    state(Namespace_01::State::non_inintialized, [&stream](auto & str) { stream << str << std::endl; });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    EXPECT_NO_THROW(state = Namespace_01::State::connecting);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NO_THROW(state = Namespace_01::State::connected);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    EXPECT_NO_THROW(state = Namespace_01::State::error);
  } // <--- state destruction

  EXPECT_GT(stream.str().length(), static_cast<size_t>(150));

  std::cout << stream.str();
}


TEST(StateReportableTest, Test_03)
{
  std::stringstream stream;

  {
    State<Namespace_02::State, std::chrono::milliseconds>
    state(Namespace_02::State::none, [&stream](auto & str) { stream << str << std::endl; });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    EXPECT_NO_THROW(state = Namespace_02::State::preparing);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NO_THROW(state = Namespace_02::State::working);
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    EXPECT_NO_THROW(state = Namespace_02::State::finished);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    EXPECT_NO_THROW(state = Namespace_02::State::error);
  } // <--- state destruction

  EXPECT_GT(stream.str().length(), static_cast<size_t>(180));

  std::cout << stream.str();
}




#include "main.h"