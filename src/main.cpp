#include <iostream>
#include "nlp/nlp.hpp"

static std::string SanitizeInput(std::string s)
{
  s.erase(std::remove_if(s.begin(), s.end(), [](char c) {return c == '\'' || c == '\"';} ),s.end());
  return s;
}

struct ExecuteConfig {
std::string text;
std::string username;
};

static ExecuteConfig ParseRuntimeArguments(int argc, char** argv)
{
  ExecuteConfig config{};

  for (int i = 1; i < argc; i++)
  {
    const std::string argument = SanitizeInput(argv[i]);
    // if (argument.find("--header") == 0) {
    //   config.message = argument.substr(9);
    //   continue;
    // }
    // else
    if (argument.find("--description") == 0)
    {
      config.text = argument.substr(14);
      continue;
    }
    // else
    // if (argument.find("--filename") == 0) {
    //   config.file_paths.emplace_back(argument.substr(11));
    //   continue;
    // }
    else
    if (argument.find("--username") == 0) {
      config.username = argument.substr(11);
      continue;
    }
    // else
    // if (argument.find("--execute_bot") == 0) {
    //   config.execute_bot = (argument.substr(14).compare("true") == 0);
    //   continue;
    // }
    // else
    // if (argument.find("--prefer_media") == 0)
    // {
    //   config.prefer_media = (argument.substr(15) == "true");
    //   continue;
    // }
    // else
    // if (argument.find("--max") == 0)
    //   config.max_results = std::stoi(argument.substr(6));
  }

  return config;
}

int main(int argc, char** argv)
{
  ExecuteConfig config = ParseRuntimeArguments(argc, argv);

  if (config.text.empty())
    throw std::invalid_argument{"Must provide text for parsing"};

  // const std::string tokenized_text = conversation:: // TODO: JSON items which can be parsed by KIQ after
  const std::string tokens_json = conversation::TokensToJSON(conversation::GetTokens(config.text));
  conversation::log(tokens_json);


  return 0;
}
