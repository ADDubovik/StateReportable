#include <gtest/gtest.h>

#include "StateToFileConfig.h"

#include <fstream>


class SettingsCreator_RAII
{
public:
  SettingsCreator_RAII()
  {
    std::ofstream(getPath()) << "StateReportable.output";
  }
  ~SettingsCreator_RAII()
  {
    std::filesystem::remove_all(getPath());
  }

  std::filesystem::path getPath() const { return std::filesystem::path("test.config"); };
};


TEST(StateToFileConfigTest, Test_01)
{
  EXPECT_NO_THROW(
    auto result = StateReportable::readFilenameFromFile("test.config");
    EXPECT_FALSE(result);
  );
}


TEST(StateToFileConfigTest, Test_02)
{
  EXPECT_NO_THROW(
    SettingsCreator_RAII creator;
    auto result = StateReportable::readFilenameFromFile("test.config");
    EXPECT_EQ(result, "StateReportable.output");
  );
}


#include "main.h"