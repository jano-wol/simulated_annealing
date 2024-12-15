#include <io/DirectoryIterator.h>

using namespace sa::io;

void DirectoryIterator::iterate(std::function<void(const std::string&)> callback) const
{
  for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) {
    if (entry.is_regular_file()) {
      callback(entry.path().string());
    }
  }
}
