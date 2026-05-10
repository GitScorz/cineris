#include "level_loader.h"
#include "paths.h"
#include <iostream>

auto LevelLoader::load(const std::string& fileName) -> std::vector<std::string> {
  std::string fullPath = Paths::Levels + fileName + ".level";
  std::ifstream file(fullPath);

  if (!file.is_open()) {
    std::cerr << "Failed to open level file: " << fullPath << std::endl;
    return {};
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty() && !line.starts_with("//")) // ignroe comments
      lines.push_back(line);
  }

  std::cout << "Loaded level: \"" << fileName << "\" successfully" << std::endl;

  return lines;
}