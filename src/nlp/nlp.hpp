#pragma once
#include <random>
#include <string>
#include <string_view>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <type_traits>
#include "types.hpp"
#include "request.hpp"

namespace conversation
{
using phrases_t = std::vector<std::string>;
phrases_t extract_phrases (const std::string& s);
class verb;
using word_map_t = std::map<std::string, verb*>;
using prep_map_t = std::map<std::string, bool>;

extern const word_map_t g_verb_map;
extern const prep_map_t g_prep_map;

template <typename... Args>
static void log(Args... args)
{
  for (const auto& arg : {args...})
    std::cout << arg;
  std::cout << std::endl;
}

template <typename T = float>
const auto GetRandom = [](const T& min, const T& max) -> T
{
  if constexpr (std::is_same_v<T, float>)
  {
    static std::random_device                rd;
    static std::default_random_engine        e{rd()};
    static std::uniform_real_distribution<T> dis(min, max);
    return dis(e);
  }
  else
  {
    static std::random_device                rd;
    static std::default_random_engine        e{rd()};
    static std::uniform_int_distribution<T>  dis(min, max);
    return dis(e);
  }
};

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
std::string  word;
float        score;
};
struct Sentiment
{
SentimentType type;
float         score;
std::vector<Keyword> keywords;

std::string GetJSON() const
{
  nlohmann::json json{};
  json["type"] = (type == SentimentType::positive) ? "positive" : "negative";
  json["score"] = score;
  json["keywords"] = nlohmann::json::array();
  for (const auto& keyword : keywords)
    json["keywords"].emplace_back(nlohmann::json::object({{"word", keyword.word}, {"score", keyword.score}}));
  return json.dump();
}

static Sentiment GetStub()
{
  return Sentiment{
    .type = (GetRandom<int>(0, 1)) ? SentimentType::positive : SentimentType::negative,
    .score = GetRandom<float>(0.0f, 1.0f),
    .keywords = {Keyword{.word = "Your mother", .score = GetRandom<float>(0.0f, 1.0f)}}
  };
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

  auto s = data.dump();

  log(s);

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

static Emotion GetStub()
{
  Emotion emotion{};
    emotion.emotions = {"joy", "fear", "disgust"};
    emotion.scores.joy      = GetRandom<float>(0.0f, 1.0f);
    emotion.scores.sadness  = GetRandom<float>(0.0f, 1.0f);
    emotion.scores.surprise = GetRandom<float>(0.0f, 1.0f);
    emotion.scores.fear     = GetRandom<float>(0.0f, 1.0f);
    emotion.scores.anger    = GetRandom<float>(0.0f, 1.0f);
    emotion.scores.disgust  = GetRandom<float>(0.0f, 1.0f);
  return emotion;
}
};

std::string        TokenizeText(std::string s);
std::vector<Token> SplitTokens(std::string s);
std::vector<Token> GetTokens(const std::string& s);
Token              ParseToken(const std::string& s);
TokenType          GetType(const std::string& type);
Sentiment          GetSentiment(const std::string& s);
Emotion            GetEmotion(const std::string& query);
nlohmann::json     TokensToJSON(const std::vector<Token>& tokens);
std::string        ToLower(std::string s);
std::string        FindVerb(const std::string&);
std::string        FindPreposition(const std::string&);


class NLP
{
public:
using context = std::pair<ObjectiveContext, SubjectiveContext>;
NLP(const std::string& username);

Message*         Insert(Message&& node, const std::string& name = "default");
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
