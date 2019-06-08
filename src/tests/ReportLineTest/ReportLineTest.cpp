#include <gtest/gtest.h>

#include "core/ReportLine.h"


using namespace StateReportable;


TEST(ReportLineTest, Test_01)
{
  auto to = std::string("to");
  // mixed char*, rvalue and lvalue
  auto line = core::ReportLine("name", std::string("from"), to, 2759872395379);
  EXPECT_EQ(line.typeName, "name");
  EXPECT_EQ(line.stateFrom, "from");
  EXPECT_EQ(line.stateTo, "to");
  EXPECT_EQ(line.duration, 2759872395379);
}


TEST(ReportLineTest, Test_02)
{
  auto line = core::ReportLine("name", "from", "to", 652938768739528);
  std::ostringstream buf;
  buf << line;
  EXPECT_EQ(buf.str(), "name\tfrom\tto\t652938768739528");
}


TEST(ReportLineTest, Test_03)
{
  auto expected = core::ReportLine("name", "from", "to", 652938768739528);
  std::ostringstream buf;
  buf << expected;
  EXPECT_NO_THROW(
    auto result = core::convertToReportLine(buf.str());
    EXPECT_EQ(result, expected);
  );
}


TEST(ReportLineTest, Test_04)
{
  auto expected = core::ReportLine("name", "from", "to", 652938768739528);

  EXPECT_NE(expected, core::ReportLine("jhgj", "from", "to", 652938768739528));
  EXPECT_NE(expected, core::ReportLine("name", "yfuewt", "to", 652938768739528));
  EXPECT_NE(expected, core::ReportLine("name", "from", "iuyiye", 652938768739528));
  EXPECT_NE(expected, core::ReportLine("name", "from", "to", 8765893728273767));
}


#include "main.h"