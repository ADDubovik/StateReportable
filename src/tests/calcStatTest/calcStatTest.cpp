#include <gtest/gtest.h>

#include "core/calcStat.h"


using namespace StateReportable;
using namespace StateReportable::core;


TEST(calcStatTest, Test_01)
{
  RawStat rawStat;
  EXPECT_NO_THROW(
    EXPECT_EQ(calcStat(rawStat), FullStat());
  );

  EXPECT_NO_THROW(
    std::cout << FullStat() << std::endl;
  );
}


TEST(calcStatTest, Test_02)
{
  RawStat rawStat = {{"enum Type::Type01", "state_from", "state_to", 300}};

  FullStat expected = {
      {"enum Type::Type01", {{{"state_from", "state_to"}, {1u, 300, 300, 300}}}}
  };

  auto fullStat = calcStat(rawStat);

  EXPECT_EQ(fullStat, expected);

  EXPECT_NO_THROW(
    std::cout << fullStat << std::endl;
  );
}


TEST(calcStatTest, Test_03)
{
  RawStat rawStat = {
      {"enum Type::Type01", "state_from", "state_to", 300},
      {"enum Type::Type01", "state_from", "state_to", 1000}
  };

  FullStat expected = {
      {"enum Type::Type01", {{{"state_from", "state_to"}, {2u, 300, 1000, 1300}}}}
  };

  auto fullStat = calcStat(rawStat);

  EXPECT_EQ(fullStat, expected);

  EXPECT_NO_THROW(
    std::cout << fullStat << std::endl;
  );
}


TEST(calcStatTest, Test_04)
{
  RawStat rawStat = {
      {"enum Type::Type01", "state_from", "state_to", 300},
      {"enum Type::Type01", "state_from", "state_to", 1000},
      {"enum Type::Type01", "state_from", "state_to", 200}
  };

  FullStat expected = {
      {"enum Type::Type01", {{{"state_from", "state_to"}, {3u, 200, 1000, 1500}}}}
  };

  auto fullStat = calcStat(rawStat);

  EXPECT_EQ(fullStat, expected);

  EXPECT_NO_THROW(
    std::cout << fullStat << std::endl;
  );
}


TEST(calcStatTest, Test_05)
{
  RawStat rawStat = {
      {"enum Type::Type01", "state_from", "state_to", 300},
      {"enum Type::Type01", "state_from", "state_to", 1000},
      {"enum Type::Type01", "state_from", "state_to", 200},
      // another type
      {"enum Type::Type02", "state_from", "state_to", 5000}
  };

  FullStat expected = {
      {"enum Type::Type01", {{{"state_from", "state_to"}, {3u,  200, 1000, 1500}}}},
      {"enum Type::Type02", {{{"state_from", "state_to"}, {1u, 5000, 5000, 5000}}}}
  };

  auto fullStat = calcStat(rawStat);

  EXPECT_EQ(fullStat, expected);

  EXPECT_NO_THROW(
    std::cout << fullStat << std::endl;
  );
}


TEST(calcStatTest, Test_06)
{
  RawStat rawStat = {
      {"enum Type::Type01", "state_from", "state_to", 300},
      {"enum Type::Type01", "state_from", "state_to", 1'000},
      {"enum Type::Type01", "state_from", "state_to", 200},
      {"enum Type::Type01", "from", "to",             10'000},
      // another type
      {"enum Type::Type02", "state_from", "state_to", 5'000},
      {"enum Type::Type02", "from", "to",             20'000}
  };

  FullStat expected = {
        {"enum Type::Type01",
            {
              {{"state_from", "state_to"}, {3u,    200,  1'000,  1'500}},
              {{"from", "to"},             {1u, 10'000, 10'000, 10'000}}
            }
        },
        {"enum Type::Type02",
            {
              {{"state_from", "state_to"}, {1u,  5'000,  5'000,  5'000}},
              {{"from", "to"},             {1u, 20'000, 20'000, 20'000}}
            }
        }
  };

  auto fullStat = calcStat(rawStat);

  EXPECT_EQ(fullStat, expected);

  EXPECT_NO_THROW(
    std::cout << fullStat << std::endl;
  );
}


TEST(calcStatTest, Test_07)
{
  RawStat rawStat = {
      {"enum Type::Type01", "state_from", "state_1", 100},
      {"enum Type::Type01", "state_from", "state_2", 200},
      {"enum Type::Type01", "state_from", "state_3", 300},
      {"enum Type::Type01", "state_from", "state_4", 400},
      // another type
      {"enum Type::Type02", "state_11", "state_to", 1'000},
      {"enum Type::Type02", "state_12", "state_to", 2'000},
      {"enum Type::Type02", "state_13", "state_to", 3'000},
      {"enum Type::Type02", "state_14", "state_to", 4'000},
      // another type
      {"enum Type::TypeNew", "state", "a", 10},
      {"enum Type::TypeNew", "state", "b", 20},
      {"enum Type::TypeNew", "state", "c", 30},
      {"enum Type::TypeNew", "state", "d", 40},
      {"enum Type::TypeNew", "state", "e", 50},
      {"enum Type::TypeNew", "state", "f", 60},
  };

  FullStat expected = {
        {"enum Type::Type01",
            {
              {{"state_from", "state_1"}, {1u, 100,  100,  100}},
              {{"state_from", "state_2"}, {1u, 200,  200,  200}},
              {{"state_from", "state_3"}, {1u, 300,  300,  300}},
              {{"state_from", "state_4"}, {1u, 400,  400,  400}}
            }
        },
        {"enum Type::Type02",
            {
              {{"state_11", "state_to"}, {1u,  1'000,  1'000,  1'000}},
              {{"state_12", "state_to"}, {1u,  2'000,  2'000,  2'000}},
              {{"state_13", "state_to"}, {1u,  3'000,  3'000,  3'000}},
              {{"state_14", "state_to"}, {1u,  4'000,  4'000,  4'000}}
            }
        },
        {"enum Type::TypeNew",
            {
              {{"state", "a"}, {1u,  10,  10,  10}},
              {{"state", "b"}, {1u,  20,  20,  20}},
              {{"state", "c"}, {1u,  30,  30,  30}},
              {{"state", "d"}, {1u,  40,  40,  40}},
              {{"state", "e"}, {1u,  50,  50,  50}},
              {{"state", "f"}, {1u,  60,  60,  60}},
            }
        }
  };

  auto fullStat = calcStat(rawStat);

  EXPECT_EQ(fullStat, expected);

  EXPECT_NO_THROW(
    std::cout << fullStat << std::endl;
  );
}


#include "main.h"