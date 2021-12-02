#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include "INIReader.h"

namespace conversation
{
static const std::string get_executable_cwd();
static const char* filename{"nlp.stats.json"};
static const auto  path = get_executable_cwd() + filename;

static const std::string get_executable_cwd()
{
  static const char* path = realpath("/proc/self/exe", NULL);
  static const char* name = basename(path);
  return std::string{path, path + strlen(path) - strlen(name)};
}

static void SaveToFile(std::string data, std::string path) {
  std::ofstream o{path};
  o << data;
}

static void SaveStats(const std::string& stats)
{
  SaveToFile(stats, path);
}

static std::string ReadStats()
{
  std::ifstream f{path};
  std::stringstream fs{};
  fs << f.rdbuf();
  return fs.str();
}

}