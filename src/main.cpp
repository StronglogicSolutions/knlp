#include <iostream>
#include "nlp/nlp.hpp"

static std::string SanitizeInput(std::string s)
{
  s.erase(std::remove_if(s.begin(), s.end(), [](char c) {return c == '\'' || c == '\"';} ),s.end());
  return s;
}

enum class Command
{
  entity      = 0x00,
  emotion     = 0x01,
  sentiment   = 0x02,
  context     = 0x03,
  verb        = 0x04,
  preposition = 0x05,
  none        = 0x06
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
    else
    if (argument.find("verb") == 0)
    {
      config.command = Command::verb;
      continue;
    }
    else
    if (argument.find("preposition") == 0)
    {
      config.command = Command::preposition;
      continue;
    }
  }

  return config;
}
//--------------------------------------------------
nlohmann::json
to_json(const conversation::Message& msg, const conversation::Tokens& tokens)
{
  nlohmann::json data;
  const conversation::NLP::context ctx{*msg.objective, *msg.subjective};
  data["message"]    = msg.get_text();
  data["entities"]   = conversation::TokensToJSON(tokens);
  data["objective"]  = ctx.first.toString();
  data["subjective"] = ctx.second.toString();
  return data;
}
//--------------------------------------------------
std::string
get_context(const std::string& text)
{
  using namespace conversation;

  NLP            nlp{"kiq"};
  nlohmann::json arr     = nlohmann::json::array();
  const auto     tokens  = GetTokens(text);
  Message*       message = nlp.Insert(Message{text, false, nullptr, nullptr, nullptr, tokens}, "kiq");
  arr.push_back(to_json(*message, tokens));
  for (const auto& msg : message->expanded)
    arr.push_back(to_json(msg, tokens));

  return arr.dump();
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
    case (Command::entity):      std_output = conversation::TokensToJSON(conversation::GetTokens(config.text)).dump();
    break;
    case (Command::sentiment):   std_output = conversation::GetSentiment(config.text).GetJSON();
    break;
    case (Command::emotion):     std_output = conversation::GetEmotion(config.text).GetJSON();
    break;
    case (Command::context):     std_output = get_context(config.text);
    break;
    case (Command::verb):        std_output = conversation::FindVerb(config.text);
    break;
    case (Command::preposition): std_output = conversation::FindPreposition(config.text);
    break;
  }

  conversation::log(std_output);

  return 0;
}
