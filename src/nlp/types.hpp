#pragma once

#include <deque>
#include <map>
#include <string>
#include <stdio.h>
#include "nlp.hpp"

namespace conversation
{
enum TokenType {
  location       = 0x00,
  person         = 0x01,
  organization   = 0x02,
  unknown        = 0x03
};

const std::vector<std::string> TOKEN_TYPES{
  "Location",
  "Person",
  "Organization",
  "Unknown"
};

struct Token {
  TokenType   type;
  std::string value;
};

struct CompositeContext {
std::string user;
std::string subject;
CompositeContext(std::string user_name, std::string subject_name) : user(user_name), subject(subject_name) {}

bool operator <(const CompositeContext &rhs) const {
  auto user_comp = user.compare(rhs.user);
  if (user_comp < 0) {
    return true;
  }
  else
  if (user_comp > 0) {
    return false;
  }
  else {
    auto subject_comp = subject.compare(rhs.subject);
    if (subject_comp < 0) {
      return true;
    }
    else
      return true;
  }
}
};

enum ProbeType {
  UNKNOWN = 0,
  WHAT = 1,
  WHERE = 2,
  WHY = 3,
  WHO = 4,
  WHEN = 5,
  HOW = 6,
  CAN = 7,
  COULD = 8,
  IS = 9,
  TRANSLATE = 10
};

// namespace constants {
const uint8_t PRTYPE_Unknown_INDEX = 0;
const uint8_t PRTYPE_unknown_INDEX = 1;
const uint8_t PRTYPE_What_INDEX = 2;
const uint8_t PRTYPE_what_INDEX = 3;
const uint8_t PRTYPE_Where_INDEX = 4;
const uint8_t PRTYPE_where_INDEX = 5;
const uint8_t PRTYPE_Why_INDEX = 6;
const uint8_t PRTYPE_why_INDEX = 7;
const uint8_t PRTYPE_Who_INDEX = 8;
const uint8_t PRTYPE_who_INDEX = 9;
const uint8_t PRTYPE_When_INDEX = 10;
const uint8_t PRTYPE_when_INDEX = 11;
const uint8_t PRTYPE_How_INDEX = 12;
const uint8_t PRTYPE_how_INDEX = 13;
const uint8_t PRTYPE_Can_INDEX = 14;
const uint8_t PRTYPE_can_INDEX = 15;
const uint8_t PRTYPE_Could_INDEX = 16;
const uint8_t PRTYPE_could_INDEX = 17;
const uint8_t PRTYPE_Is_INDEX = 18;
const uint8_t PRTYPE_is_INDEX = 19;
const uint8_t PRTYPE_Translate_INDEX = 20;
const uint8_t PRTYPE_translate_INDEX = 21;
const uint8_t PRTYPE_Whose_INDEX = 22;

const std::vector<std::string> PRTypeNames{
  "Unknown",
  "unknown",
  "What",
  "what",
  "Where",
  "where",
  "Why",
  "why",
  "Who",
  "who",
  "When",
  "when",
  "How",
  "how",
  "Can",
  "can",
  "Could",
  "could",
  "Is",
  "is",
  "Translate",
  "translate",
  "whose"
};

bool      IsQuestion(const std::string& s);
ProbeType DetectProbeType(const std::string& s);

struct ObjectiveContext
{
bool         is_continuing;
bool         is_question;
ProbeType    probe_type;

static ObjectiveContext Create(const std::string& s)
{
  ObjectiveContext context{};
  context.is_question = IsQuestion(s);
  context.probe_type  = DetectProbeType(s);
  return context;
}

std::string toString()
{
  if (is_question)
  {
    auto pr_index = (probe_type == PRTYPE_Unknown_INDEX) ? 1 : (probe_type * 2);
    return "Is " + PRTypeNames.at(pr_index) + " question";
  }
  if (is_continuing)
  {
    return "Is a continuation";
  }

  return "Unknown";
}
};

struct SubjectiveContext
{
SubjectiveContext(const std::vector<Token>& tokens)
{
  if (!tokens.empty())
  {
    uint8_t i{};
    for (auto it = tokens.begin(); (it != tokens.end()) && (i < 3); i++)
      subjects[i] = (it++)->value;
  }
}

SubjectiveContext(std::string subject)
{
  subjects[0] = subject;
  subjects[1] = "";
  subjects[2] = "";
}

const std::string& operator[] (uint8_t i) const
{
  if (i < 3)
    return subjects[i];
  return subjects[0];
}

const std::string Current() const
{
  if (idx == 0)
    return subjects[2];
  return subjects[idx - 1];
}

const std::string Next()
{
  const std::string ret_s = subjects[idx];
  idx = (idx == 2) ? 0 : idx + 1;
  return ret_s;
}

void Insert(std::string s)
{
  subjects[idx] = s;

  (idx == 2) ? idx = 0 : idx++;
}

std::string toString()
{
  std::string s{};
  s.reserve(subjects[0].size() + subjects[1].size() + subjects[2].size());

  s += subjects[0];

  if (!subjects[1].empty())
    s +=  ", " + subjects[1];
  if (!subjects[2].empty())
    s +=  ", " + subjects[2];

  return s;
}

std::string subjects[3];
uint8_t     idx{0};
};

struct Message;
using  Map               = std::map<const std::string, Message*>;
using  MessageObjects    = std::deque<Message>;
using  SubjectContexts   = std::deque<SubjectiveContext>;
using  ObjectiveContexts = std::deque<ObjectiveContext>;
using  Tokens            = std::vector<Token>;

struct Message
{
const std::string         text;
const bool                received;
      Message*            next;
      SubjectiveContext*  subjective;
      ObjectiveContext*   objective;
      Tokens              tokens;
};

} // namespace conversation
