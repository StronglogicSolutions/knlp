#include <iostream>
#include "nlp/nlp.hpp"

static std::string SanitizeInput(std::string s)
{
  s.erase(std::remove_if(s.begin(), s.end(), [](char c) {return c == '\'' || c == '\"';} ),s.end());
  return s;
}

enum class Command
{
  entity    = 0x00,
  emotion   = 0x01,
  sentiment = 0x02,
  context   = 0x03,
  none      = 0x04
};

struct ExecuteConfig {
std::string text;
std::string username;
Command     command{Command::none};

bool valid() const
{
  return command != Command::none;
}
};

static ExecuteConfig ParseRuntimeArguments(int argc, char** argv)
{
  ExecuteConfig config{};

  for (int i = 1; i < argc; i++)
  {
    const std::string argument = SanitizeInput(argv[i]);
    if (argument.find("--description") == 0)
    {
      config.text = argument.substr(14);
      continue;
    }
    else
    if (argument.find("--username") == 0)
    {
      config.username = argument.substr(11);
      continue;
    }
    else
    if (argument.find("entity") == 0)
    {
      config.command = Command::entity;
      continue;
    }
    else
    if (argument.find("sentiment") == 0)
    {
      config.command = Command::sentiment;
      continue;
    }
    else
    if (argument.find("emotion") == 0)
    {
      config.command = Command::emotion;
      continue;
    }
    else
    if (argument.find("context") == 0)
    {
      config.command = Command::context;
      continue;
    }
  }

  return config;
}

std::string
to_json(const conversation::NLP::context& ctx)
{
  nlohmann::json data;
  data["objective"]  = ctx.first.toString();
  data["subjective"] = ctx.second.toString();
  return data.dump();
}

std::string
get_context(const std::string& text)
{
  using namespace conversation;
  NLP nlp{"kiq"};
  Message* message = nlp.Insert(Message{.text = text, .received = false, .tokens = GetTokens(text)}, "kiq");
  return to_json(NLP::context{*message->objective, *message->subjective});
}

//--------------------------------------------------
int main(int argc, char** argv)
{
  ExecuteConfig config = ParseRuntimeArguments(argc, argv);
  std::string   std_output;

  if (!config.valid())
    throw std::invalid_argument{"No command provided"};
  if (config.text.empty())
    throw std::invalid_argument{"No text provided"};

  switch (config.command)
  {
    case (Command::entity):
      std_output = conversation::TokensToJSON(conversation::GetTokens(config.text));
    break;
    case (Command::sentiment):
      std_output = conversation::GetSentiment(config.text).GetJSON();
    break;
    case (Command::emotion):
      std_output = conversation::GetEmotion(config.text).GetJSON();
    break;
    case (Command::context):
      std_output = get_context(config.text);
    break;
  }

  conversation::log(std_output);

  return 0;
}
