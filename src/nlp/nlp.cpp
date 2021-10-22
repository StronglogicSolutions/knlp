#include "nlp.hpp"
#include "util.hpp"
#include <algorithm>

namespace conversation {
static const char* SENTIMENT_API_TOKEN{};
static const uint8_t SENTIMENT_ANALYZER_INDEX{0x00};
static const char* URLS[]{
  "https://twinword-sentiment-analysis.p.rapidapi.com/analyze/"
};

static const std::string TOKENIZER_PATH{"third_party/MITIE/tools/ner_stream/ner_stream"};
static const std::string MODEL_PATH{"third_party/MITIE/MITIE-models/english/ner_model.dat"};
static const std::string TOKEN_FILE_NAME{"tokenized_message.txt"};

static const std::string get_prefix()
{
  return get_executable_cwd() + "../";
}

std::string TokenizeText(std::string s)
{

  static const std::string execution_endpoint{get_prefix() + TOKENIZER_PATH + ' ' + get_prefix() + MODEL_PATH + '>' + TOKEN_FILE_NAME};
         const std::string execution_line    {"echo \"" + s + "\" | " + execution_endpoint};

  std::system(execution_line.c_str());

  return static_cast<std::stringstream const&>(std::stringstream() << std::ifstream("tokenized_message.txt").rdbuf()).str();
}


static std::string ToLower(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), [](char c) { return tolower(c); });
  return s;
}
/**
 * GetType
 *
 * @param
 * @returns
 *
 */
TokenType GetType(const std::string& type)
{
  if (type.compare("LOCATION") == 0)
    return TokenType::location;
  if (type.compare("PERSON") == 0)
    return TokenType::person;
  if (type.compare("ORGANIZATION") == 0)
    return TokenType::organization;
  return TokenType::unknown;
}

/**
 * ParseToken
 *
 * @param
 * @returns
 *
 */
Token ParseToken(const std::string& s)
{
  auto delim = s.find(' ');
  return Token{
    .type  = GetType(s.substr(0, delim)),
    .value = s.substr(delim + 1)
  };
}

/**
 * SplitTokens
 *
 * @param
 * @returns
 *
 */
std::vector<Token> SplitTokens(std::string s)
{
  std::vector<Token> tokens{};
  auto               delim_index = s.find_first_of('[');

  while (delim_index != std::string::npos)
  {
    const auto token_start     = s.substr(delim_index);
    const auto delim_end_index = (token_start.find_first_of(']') - 1);
    const auto token_value     = token_start.substr(1, delim_end_index);

    tokens.push_back(ParseToken(token_value));

    if (token_start.size() >= (token_value.size() + 3))
    {
      s           = token_start.substr(token_value.size() + 3);
      delim_index = s.find_first_of('[');
    }
    else
      break;
  }
  return tokens;
}

/**
 * DetectQuestionType
 *
 * @param
 * @returns
 */
ProbeType DetectProbeType(const std::string& s)
{
  uint8_t num = conversation::PRTypeNames.size();
  for (uint8_t i = 2; i < num; i++)
    if (s.find(conversation::PRTypeNames.at(i)) != std::string::npos)
      return static_cast<conversation::ProbeType>((i / 2));
  return conversation::ProbeType::UNKNOWN;
}

/**
 * IsQuestion
 */
bool IsQuestion(const std::string& s)
{
  return (s.find("?") != std::string::npos);
}

bool IsContinuing(Message* node) {
  while (node->next != nullptr) {
    if (node->next->received == false) {
      return true;
    }
    node = node->next;
  }
  return false;
}



static int32_t GetSentimentCount()
{
  int32_t     count;
  std::string file = ReadStats();
  if (file.empty())
    count = 0;
  else
    count = std::stoi(file);
  return count;
};

static void IncrementSentimentCount()
{
  int32_t count = GetSentimentCount();
  SaveStats(std::to_string(++count));
};
/**
 *
 */
Sentiment GetSentiment(const std::string& query)
{
  Sentiment sentiment{};
  RequestResponse response{cpr::Post(
    cpr::Url       {URLS[SENTIMENT_ANALYZER_INDEX]},
    cpr::Header    {{"x-rapidapi-host", "twinword-sentiment-analysis.p.rapidapi.com"},
                    {"x-rapidapi-key" , SENTIMENT_API_TOKEN}},
    cpr::Parameters{{"text", query}})};

  if (!response.error)
  {
    IncrementSentimentCount();
    const auto json = response.json();
    if (!json.is_null() && json.is_object())
    {
      sentiment.type  = StringToSentiment(json["type"] .get<std::string>());
      sentiment.score = json["score"].get<float>();

      for (const auto& item : json["keywords"])
        sentiment.keywords.emplace_back(Keyword{
          .score = item["score"].get<float>(),
          .word  = item["word"].get<std::string>()});
    }
  }
  else
    log("GetSentiment request failed.", response.GetError().c_str());

  return sentiment;
}

/**
 *
 */
Message* NLP::Insert(Message&& node, std::string name, std::string subject)
{
  m_q.emplace_back(std::move(node));
  Message* node_ref = &m_q.back();
  const Map::const_iterator it = m_m.find(name);

  if (it == m_m.end())                                       // New
  {
    m_o.emplace_back(std::move(ObjectiveContext{}));
    m_s.emplace_back(std::move(SubjectiveContext{subject}));
    ObjectiveContext*  o_ctx_ref = &m_o.back();
    SubjectiveContext* s_ctx_ref = &m_s.back();
    node_ref->objective          = o_ctx_ref;
    node_ref->subjective         = s_ctx_ref;
    node_ref->next               = nullptr;

    m_m.insert({name, node_ref});
  }
  else                                                       // Append
  {
    Message* previous_head       = &(*it->second);
    while (previous_head->next)
      previous_head = previous_head->next;
    previous_head->next          = node_ref;
    node_ref->next               = nullptr;
    node_ref->objective          = previous_head->objective;
    node_ref->subjective         = previous_head->subjective;
    node_ref->subjective->Insert(subject);

    // m_m.erase(it);
    // m_m.insert({name, node_ref});
  }

  return node_ref;
}


/**
 * Reply
 */
void NLP::Reply(Message* node, std::string reply, std::string name) {
  Message reply_node{
    .text       = reply,
    .received   = false,
    .next       = node->next,
    .subjective = node->subjective,
    .objective  = node->objective
  };

  m_q.emplace_back(std::move(reply_node));
  Message* reply_node_ref = &m_q.back();

  node->next = reply_node_ref;
}

template<typename T>
class reverse
{
private:
  T& iterable_;
public:
  explicit reverse(T& iterable) : iterable_{iterable} {}
  auto begin() const { return std::rbegin(iterable_); }
  auto end() const { return std::rend(iterable_); }
};

static std::string GetSubjects(Message* node)
{
  const auto IsUnknown = [](const std::string& s) -> bool { return s == "unknown"; };
  static const             int8_t max{5};
  std::vector<std::string> subjects{};

  while ((node != nullptr) && (subjects.size() < max))
  {
    std::string subject = node->subjective->Next();
    while (!subject.empty() && !IsUnknown(subject))
    {
      subjects.push_back(subject);
      subject = node->subjective->Next();
    }
    node = node->next;
  }

  std::string ret_s{};
  if (subjects.size())
  {
    for (const auto& s : subjects)
      ret_s += s + ", ";
      return ret_s.substr(0, ret_s.size() - 2);
  }
  return "";
}
/**
 * toString
 */
std::string NLP::toString() {
  std::string node_string{};
  for (const auto& it : m_m)
   {
    const std::string interlocutor = it.first;
          Message*    node         = it.second;

    node_string +=
"┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n│                                                                                                                    │\n\
│░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│\n\
│░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░CONVERSATION░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│\n\
│░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░│\n│";

    node_string += "\n│Interlocutor: " + interlocutor +
                   "\n│Subjective:   " + GetSubjects(node) +
                   "\n│Nodes:\n" +
                   "├────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤\n" +
                   "├────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤\n";

    uint8_t n_idx{1};
    while ( node != nullptr)
    {
      node_string += "│ " + std::to_string(n_idx) + ".\n│   ";
      node_string += "Objective:  "  + node->objective->toString()  + "\n│   ";
      node_string += "Subjective: "  + node->subjective->toString() + "\n│   ";
      node_string += "From:       ";
      node_string += (node->received) ? interlocutor : GetUsername();
      node_string += "\n│   Message:    " + node->text + "\n";
      node_string += "├───────────────────────────────────────────────────────────────"
                      "─────────────────────────────────────────────────────┤\n";
      node = node->next;
      n_idx++;
    }
    node_string +=
"│                                                                                                                    │\n│                                                                                                                    │\n└────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n\n";
  }
  return node_string;
}

bool NLP::SetContext(Message* node, const Tokens& tokens)
{
  try
  {
    SubjectiveContext s_ctx{tokens};
    ObjectiveContext  o_ctx{};
    o_ctx.is_question   = IsQuestion(node->text);
    o_ctx.is_continuing = IsContinuing(node);
    o_ctx.probe_type    = DetectProbeType(node->text);

    m_o.emplace_back(std::move(o_ctx));
    m_s.emplace_back(std::move(s_ctx));
    node->objective  = &m_o.back();
    node->subjective = &m_s.back();
  }
  catch (const std::exception& e)
  {
    std::cout << "Exception caught: " << e.what() << std::endl;
    return false;
  }
  return true;
}
} // namespace conversation