#include <gtest/gtest.h>

#include "split.h"

#include <array>


TEST(splitTest, Test_01)
{
  EXPECT_NO_THROW(
    auto result = StateReportable::split("", ' ');
    EXPECT_EQ(result.size(), 0u);
  );
}



TEST(splitTest, Test_02)
{
  EXPECT_NO_THROW(
    auto result = StateReportable::split(" ", ' ');
    EXPECT_EQ(result.size(), 0u);
  );
}



TEST(splitTest, Test_03)
{
  EXPECT_NO_THROW(
    auto result = StateReportable::split("                      ", ' ');
    EXPECT_EQ(result.size(), 0u);
  );
}



TEST(splitTest, Test_04)
{
  std::vector<std::string> expected = {"first", "second", "third", "fourth"};
  EXPECT_NO_THROW(
    auto result = StateReportable::split("first\tsecond\tthird\t\t\tfourth", '\t');
    EXPECT_EQ(result.size(), 4u);
    EXPECT_EQ(result, expected);
  );
}



TEST(splitTest, Test_05)
{
  std::vector<std::string> expected = {"first", "second", "third", "fourth"};
  EXPECT_NO_THROW(
    // trailing \t added
    auto result = StateReportable::split("first\tsecond\tthird\t\t\tfourth\t", '\t');
    EXPECT_EQ(result.size(), 4u);
    EXPECT_EQ(result, expected);
  );
}


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


TEST(splitTest, GreatRandomTest)
{
  srand(static_cast<unsigned int>(time(nullptr)));

  const auto separator = '\t';
  const auto repetitions = 100;
  for ( auto rep = 0; rep < repetitions; ++rep )
  {
    std::vector<std::string> expected;
    auto words =  20 + (rand() & 0xff);
    expected.reserve(words);

    for ( auto i = 0; i < words; ++i )
      expected.emplace_back(getStringRandom());

    std::string toProcess;
    for ( auto const &elem : expected )
    {
      auto prefixSeparators = rand() & 0x3;
      auto postfixSeparators = 1 + (rand() & 0x3);

      for ( auto j = 0; j < prefixSeparators; ++j )
        toProcess += separator;

      toProcess += elem;

      for ( auto j = 0; j < postfixSeparators; ++j )
        toProcess += separator;
    }

    EXPECT_NO_THROW(
      auto result = StateReportable::split(toProcess, separator);
      EXPECT_EQ(result, expected);
    );
  }
}


#include "main.h"