#include <io/Io.h>

#include <filesystem>

using namespace sa::io;

std::string Io::workspaceRootPath =
    std::filesystem::absolute(__FILE__).parent_path().parent_path().parent_path().parent_path().parent_path();

std::string Io::dataFolderPath = std::filesystem::path(Io::workspaceRootPath).append("data");
