#include <gtest/gtest.h>

#include "toFile/ConfigReader.h"

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


using namespace StateReportable;


TEST(StateToFileConfigReaderTest, Test_01)
{
  EXPECT_NO_THROW(
    auto result = toFile::readFilenameFromFile("test.config");
    EXPECT_FALSE(result);
  );
}


TEST(StateToFileConfigReaderTest, Test_02)
{
  EXPECT_NO_THROW(
    SettingsCreator_RAII creator;
    auto result = toFile::readFilenameFromFile("test.config");
    EXPECT_EQ(result, "StateReportable.output");
  );
}


#include "main.h"