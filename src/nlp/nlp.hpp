#pragma once

#include <string>
#include <string_view>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "types.hpp"
#include "request.hpp"

namespace conversation
{
template <typename... Args>
static void log(Args... args)
{
  for (const auto& arg : {args...})
    std::cout << arg;
  std::cout << std::endl;
}
enum class SentimentType
{
positive,
negative
};

static SentimentType StringToSentiment(const std::string& s)
{
  if (s == "positive")
    return SentimentType::positive;
  return SentimentType::negative;
}

struct Keyword
{
float        score;
std::string  word;
};
struct Sentiment
{
SentimentType type;
float         score;
std::vector<Keyword> keywords;

std::string GetJSON() const
{
  return "";
}
};

struct Emotions
{
float joy;
float sadness;
float surprise;
float fear;
float anger;
float disgust;
};
struct Emotion
{
std::vector<std::string> emotions;
Emotions                 scores;

static Emotion Parse(const nlohmann::json& data)
{
  Emotion emotion{};

  if (!data.is_null() && data.is_object())
  {
    for (const auto& item : data["emotions_detected"])
      emotion.emotions.emplace_back(item.get<std::string>());
    emotion.scores.joy      = data["emotion_scores"]["joy"]     .get<float>();
    emotion.scores.sadness  = data["emotion_scores"]["sadness"] .get<float>();
    emotion.scores.surprise = data["emotion_scores"]["surprise"].get<float>();
    emotion.scores.fear     = data["emotion_scores"]["fear"]    .get<float>();
    emotion.scores.anger    = data["emotion_scores"]["anger"]   .get<float>();
    emotion.scores.disgust  = data["emotion_scores"]["disgust"] .get<float>();
  }

  return emotion;
}

std::string GetJSON() const
{
  nlohmann::json json{};
  json["joy"]      = std::to_string(scores.joy);
  json["sadness"]  = std::to_string(scores.sadness);
  json["surprise"] = std::to_string(scores.surprise);
  json["fear"]     = std::to_string(scores.fear);
  json["anger"]    = std::to_string(scores.anger);
  json["disgust"]  = std::to_string(scores.disgust);
  json["emotions"] = nlohmann::json::array();
  for (const auto& emotion : emotions)
    json["emotions"].emplace_back(emotion);
  return json.dump();
}
};

std::string        TokenizeText(std::string s);
std::vector<Token> SplitTokens(std::string s);
std::vector<Token> GetTokens(const std::string& s);
Token              ParseToken(const std::string& s);
TokenType          GetType(const std::string& type);
ProbeType          DetectProbeType(const std::string& s);
bool               IsQuestion(const std::string& s);
Sentiment          GetSentiment(const std::string& s);
Emotion            GetEmotion(const std::string& query);
std::string        TokensToJSON(const std::vector<Token>& tokens);


class NLP
{
public:
NLP(std::string username)
: m_username{username} {}

Message*         Insert(Message&& node, const std::string& name);
void             Reply(Message* node, std::string reply, std::string name);
bool             SetContext(Message* node, const Tokens& tokens);
Map              GetConversations() { return m_m; }
const Message*   GetConversation(std::string name) { return m_m.at(name); }
std::string      GetUsername() { return m_username; }
std::string      toString();

private:
Map                m_m;        // Pointer map
MessageObjects     m_q;        // Queue of objects
ObjectiveContexts  m_o;        // Queue of objects
SubjectContexts    m_s;        // Queue of objects
std::string        m_username;
};

} // namespace Conversation
