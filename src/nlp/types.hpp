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

bool operator <(const CompositeContext &rhs) const
{
  const auto user_comp = user.compare(rhs.user);
  return (user_comp < 0) ?
    true : (user_comp > 0) ?
      false :
      subject.compare(rhs.subject) < 0;
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
  TRANSLATE = 10,
  whose = 11,
  was = 12,
  were = 13,
  am = 14
};

// namespace constants {

const std::vector<std::string> PRTypeNames{
  "unknown",
  "what",
  "where",
  "why",
  "who",
  "when",
  "how",
  "can",
  "could",
  "is",
  "translate",
  "whose",
  "was",
  "were",
  "am"
};

const std::vector<std::string> ImperativeNames{
  "please",
  "you must",
  "do this",
  "don't",
  "make",
  "create",
  "start",
  "stop"
};

bool        IsSinglePhrase(const std::string& s);
bool        IsAssertion(const std::string& s);
std::string FindImperative(const std::string& s);
size_t      IsQuestion(const std::string& s);
ProbeType   DetectProbeType(const std::string& s);

struct message_interface
{
virtual ~message_interface()         = default;
virtual std::string get_text() const = 0;
};

struct ObjectiveContext
{
message_interface*  parent{nullptr};
bool                is_continuing{false};
bool                is_question{false};
bool                is_assertion{false};
bool                is_imperative{false};
bool                is_single_phrase{false};
ProbeType           probe_type{ProbeType::UNKNOWN};
size_t              q_index{std::string::npos};
std::string         imperative_word{""};

std::string toString() const
{
  std::vector<std::string> props;

  if (is_question)
    props.push_back("Is a question");
  if (is_continuing)
    props.push_back("Is a continuation");
  if (is_assertion)
    props.push_back("Is an assertion");
  if (is_imperative)
    props.push_back("Is an imperative statement");
  if (is_single_phrase)
    props.push_back("Is a single phrase");
  if (probe_type != ProbeType::UNKNOWN)
  {
    auto   pr_name  = PRTypeNames.at(probe_type);
    size_t in_index;
    if (is_question)
      in_index = parent->get_text().find(pr_name);

    if (!is_question || (parent->get_text().find(pr_name) > parent->get_text().find('.', in_index)))
      props.push_back("Has a probetype of " + pr_name);
  }

  if (props.empty())
    props.push_back("Unknown");

  std::string delim = "";
  std::string ret   = "";
  for (const auto& prop : props)
  {
    ret   += delim + prop;
    delim  = ",";
  }

  return ret;
}

std::string debug() const
{
  return "parent's not null: " + std::to_string(parent != nullptr) + '\n' +
         "is_single_phrase: "  + std::to_string(is_single_phrase)  + '\n' +
         "is_continuing: "     + std::to_string(is_continuing)     + '\n' +
         "is_imperative: "     + std::to_string(is_imperative)     + '\n' +
         "is_assertion: "      + std::to_string(is_assertion)      + '\n' +
         "is_question: "       + std::to_string(is_question)       + '\n' +
         "probe_type: "        + PRTypeNames.at(probe_type)        + '\n' +
         "q_index: "           + std::to_string(q_index)           + '\n' +
         "Imperative: "        + imperative_word;
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
  if (s.empty())
    return;

  subjects[idx] = s;
  (idx == 2) ? idx = 0 : idx++;
}

std::string toString() const
{
  std::string s;
  s += subjects[0];

  if (!subjects[1].empty())
    s +=  ", " + subjects[1];
  if (!subjects[2].empty())
    s +=  ", " + subjects[2];

  return s;
}

bool
empty() const
{
  for (const auto& subject : subjects)
    if (!subject.empty())
      return false;
  return true;
}

std::string subjects[3];
uint8_t     idx{0};
};

// struct Message;

using  Tokens            = std::vector<Token>;
using  SubjectContexts   = std::deque<SubjectiveContext>;
using  ObjectiveContexts = std::deque<ObjectiveContext>;
struct Message : public message_interface
{
 Message(const std::string&       text_,
               bool               received_,
               Message*           next_       = nullptr,
               SubjectiveContext* subjective_ = nullptr,
               ObjectiveContext*  objective_  = nullptr,
         const Tokens&            tokens_     = {})
 : text(text_),
   received(received_),
   next(next_),
   subjective(subjective_),
   objective(objective_),
   tokens(tokens_)
   {}

 ~Message() final = default;
 std::string get_text() const final { return text; };
 std::string find_subject(const std::string&) const;

 std::string         text;
 bool                received;
 Message*            next;
 SubjectiveContext*  subjective;
 ObjectiveContext*   objective;
 Tokens              tokens;
};

using  Map               = std::map<const std::string, Message*>;
using  MessageObjects    = std::deque<Message>;
} // namespace conversation
