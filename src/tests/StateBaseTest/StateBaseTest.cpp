#include <gtest/gtest.h>

#include "core/StateBase.h"

#include <thread>
#include <memory>


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
      case State::non_inintialized: return stream << "non_inintialized";
      case State::connecting: return stream << "connecting";
      case State::connected: return stream << "connected";
      case State::error: return stream << "error";
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


TEST(StateBaseTest, Test_01)
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


TEST(StateBaseTest, Test_02)
{
  std::stringstream stream;
  
  {
    core::StateBase<Namespace_01::State, std::chrono::milliseconds>
    state(Namespace_01::State::non_inintialized, [&stream](auto && str) { stream << str << std::endl; });

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


TEST(StateBaseTest, Test_03)
{
  std::stringstream stream;

  {
    core::StateBase<Namespace_02::State, std::chrono::milliseconds>
    state(Namespace_02::State::none, [&stream](auto && str) { stream << str << std::endl; });

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


TEST(StateBaseTest, Test_04)
{
  std::unique_ptr<core::ReportLine> line;
  
  {
    core::StateBase<Namespace_01::State, std::chrono::milliseconds>
    state(Namespace_01::State::non_inintialized, [&line](auto &&l) { line = std::make_unique<core::ReportLine>(l); });

    EXPECT_FALSE(line);

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    EXPECT_NO_THROW(state = Namespace_01::State::connecting);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_01::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "non_inintialized");
    EXPECT_EQ(line->stateTo, "connecting");
    EXPECT_GE(line->duration, 4);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NO_THROW(state = Namespace_01::State::connected);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_01::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "connecting");
    EXPECT_EQ(line->stateTo, "connected");
    EXPECT_GE(line->duration, 9);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    EXPECT_NO_THROW(state = Namespace_01::State::error);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_01::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "connected");
    EXPECT_EQ(line->stateTo, "error");
    EXPECT_GE(line->duration, 14);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NO_THROW(state = Namespace_01::State::connected);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_01::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "error");
    EXPECT_EQ(line->stateTo, "connected");
    EXPECT_GE(line->duration, 9);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  } // <--- state destruction

  EXPECT_TRUE(line);
  EXPECT_NE(line->typeName.find(typeid(Namespace_01::State).name()), std::string::npos);
  EXPECT_EQ(line->stateFrom, "connected");
  EXPECT_EQ(line->stateTo, "destruction");
  EXPECT_GE(line->duration, 19);
}


TEST(StateBaseTest, Test_05)
{
  std::unique_ptr<core::ReportLine> line;

  {
    core::StateBase<Namespace_02::State, std::chrono::milliseconds>
    state(Namespace_02::State::none, [&line](auto &&l) { line = std::make_unique<core::ReportLine>(l); });

    EXPECT_FALSE(line);

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    EXPECT_NO_THROW(state = Namespace_02::State::preparing);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_02::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "none");
    EXPECT_EQ(line->stateTo, "preparing");
    EXPECT_GE(line->duration, 4);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_NO_THROW(state = Namespace_02::State::working);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_02::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "preparing");
    EXPECT_EQ(line->stateTo, "working");
    EXPECT_GE(line->duration, 9);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    EXPECT_NO_THROW(state = Namespace_02::State::finished);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_02::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "working");
    EXPECT_EQ(line->stateTo, "finished");
    EXPECT_GE(line->duration, 14);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    EXPECT_NO_THROW(state = Namespace_02::State::error);
    EXPECT_TRUE(line);
    EXPECT_NE(line->typeName.find(typeid(Namespace_02::State).name()), std::string::npos);
    EXPECT_EQ(line->stateFrom, "finished");
    EXPECT_EQ(line->stateTo, "error");
    EXPECT_GE(line->duration, 19);

    line.reset();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } // <--- state destruction

  EXPECT_NE(line->typeName.find(typeid(Namespace_02::State).name()), std::string::npos);
  EXPECT_EQ(line->stateFrom, "error");
  EXPECT_EQ(line->stateTo, "destruction");
  EXPECT_GE(line->duration, 9);
}


#include "main.h"
