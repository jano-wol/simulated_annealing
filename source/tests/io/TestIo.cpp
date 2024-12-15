#include <gmock/gmock.h>

#include <filesystem>

#include <io/Io.h>

using namespace sa::io;

TEST(Io, FontPath) { EXPECT_TRUE(std::filesystem::exists(Io::getFontPath())); }
