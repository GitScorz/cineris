#include "level_loader.h"
#include "paths.h"

auto LevelLoader::load(const std::string& fileName) -> std::vector<std::string> {
  std::string fullPath = Paths::Levels + fileName + ".level";
  std::ifstream file(fullPath);

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty() && !line.starts_with("#")) // ignroe comments
      lines.push_back(line);
  }
  return lines;
}