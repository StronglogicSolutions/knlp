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
};

std::string        TokenizeText(std::string s);
std::vector<Token> SplitTokens(std::string s);
std::vector<Token> GetTokens(const std::string& s);
Token              ParseToken(const std::string& s);
TokenType          GetType(const std::string& type);
ProbeType          DetectProbeType(const std::string& s);
bool               IsQuestion(const std::string& s);
Sentiment          GetSentiment(const std::string& s);
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
