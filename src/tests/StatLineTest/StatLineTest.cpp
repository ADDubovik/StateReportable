#include <gtest/gtest.h>

#include "core/StatLine.h"

#include <iostream>


using namespace StateReportable::core;

TEST(StatLineTest, Test_01)
{
  StatLine statLine(500);
  EXPECT_EQ(statLine.pcs, 1u);
  EXPECT_EQ(statLine.min, 500);
  EXPECT_EQ(statLine.max, 500);
  EXPECT_EQ(statLine.total, 500);

  statLine.add(1000);
  EXPECT_EQ(statLine.pcs, 2u);
  EXPECT_EQ(statLine.min, 500);
  EXPECT_EQ(statLine.max, 1000);
  EXPECT_EQ(statLine.total, 1500);

  statLine.add(200);
  EXPECT_EQ(statLine.pcs, 3u);
  EXPECT_EQ(statLine.min, 200);
  EXPECT_EQ(statLine.max, 1000);
  EXPECT_EQ(statLine.total, 1700);

  EXPECT_NO_THROW(std::cout << statLine << std::endl);
}


TEST(StatLineTest, Test_02)
{
  StatLine statLine(10u, 20, 30, 40);
  EXPECT_EQ(statLine.pcs, 10u);
  EXPECT_EQ(statLine.min, 20);
  EXPECT_EQ(statLine.max, 30);
  EXPECT_EQ(statLine.total, 40);
}


#include "main.h"