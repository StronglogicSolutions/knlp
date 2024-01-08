#include "nlp.hpp"
#include "util.hpp"
#include <algorithm>
#include <set>
#include <concepts>

template <typename T>
void Split(const std::string &s, char delim, T result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
      *result++ = item;
}

std::vector<std::string> Split(const std::string &s, char delim)
{
    std::vector<std::string> v;
    if (s.size())
      Split(s, delim, std::back_inserter(v));
    return v;
}

template <typename T>
concept is_int_t = std::is_integral_v<T>;

namespace conversation {
static       std::string API_KEY;
static const std::string CONFIG_PATH{get_executable_cwd() + "../config/config.ini"};
static const uint8_t     SENTIMENT_ANALYZER_INDEX{0x00};
static const uint8_t     EMOTION_ANALYZER_INDEX  {0x01};
static const char*       URLS[]{
  "https://twinword-sentiment-analysis.p.rapidapi.com/analyze/",
  "https://twinword-emotion-analysis-v1.p.rapidapi.com/analyze/"
};

static const std::string TOKENIZER_PATH {"third_party/MITIE/tools/ner_stream/ner_stream"};
static const std::string MODEL_PATH     {"third_party/MITIE/MITIE-models/english/ner_model.dat"};
static const std::string TOKEN_FILE_NAME{"tokenized_message.txt"};

static const std::set<std::string_view> titles{ "Mr.", "Mrs.", "Ms.", "Dr." };

std::string unescape_breaks(std::string s)
{
  std::string output;
  if (s.empty())
    return output;

  size_t idx;
  while ((idx = s.find("\\n")) != s.npos)
  {
    output += s.substr(0, idx);
    output += '\n';
    idx    += 2;
    s       = s.substr(idx);
  }

  output += s;
  return output;
}

void Message::expand(Message&& msg)
{
  expanded.push_back(msg);
}

template <typename T>
requires is_int_t<T>
static bool is_title(std::string::const_iterator s_i, T idx)
{
  if (!idx || idx < 3)
    return false;

  for (auto t : titles)
  {
    const auto t_size = t.size();
    if (idx >= t_size)
      for (auto i = 0; i < t_size; i++)
        if (*(s_i + idx - i) != t.at(t_size - 1 - i))
          return false;

      return true;
  }

  return false;
}

auto is_soft_stop = [](const std::string& s, auto idx)
{
  if (s.empty())
    return false;
  return ((s.size() > idx + 1 ) && s.at(idx + 1) == '.' ||
          ((idx > 0)            && s.at(idx - 1) == '.'));
};

static bool initialize()
{
  static INIReader config{CONFIG_PATH};
         API_KEY = config.GetString("knlp", "api_key", "");
  if (API_KEY.empty())
    throw std::invalid_argument{"Please set an API key in config"};
  return true;
}

phrases_t extract_phrases (const std::string& t)
{
  static const std::set<char> delimiters = {'\n', '.', ';', ':'};
         const auto           s          = unescape_breaks(t);         // <--- INPUT STRING
         const auto           size       = s.size();

  auto handler = [&s](char c, auto i)
  {
    if ((c == '.') && is_title(s.begin(), i))
      return 0;

    char   d;
    while  ((++i < s.size()) && ((d = s.at(i)) == c));
    return i;
  };

  phrases_t  phrases;
  int        j{};

  for (int i = 0, end = 0; i < size;)
  {
    char c = s.at(i);
    if ((delimiters.find(c) != delimiters.end()) && (end = handler(c, i)))
    {
      phrases.push_back(s.substr(j, (end - j)));
      j = i = end;
    }
    else
      i++;
  }

  if (const auto last_phrase = s.substr(j); !last_phrase.empty())
    phrases.push_back(last_phrase);

  return (phrases.size() > 1) ? phrases : phrases_t{};
};

static const std::string get_prefix()
{
  return get_executable_cwd() + "../";
}

bool initialized = initialize();

std::string TokenizeText(std::string s)
{
  static const std::string execution_endpoint{get_prefix() + TOKENIZER_PATH + ' ' + get_prefix() + MODEL_PATH + '>' + TOKEN_FILE_NAME};
         const std::string execution_line    {"echo \"" + s + "\" | " + execution_endpoint + " 2>/dev/null"};

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

void strip_char(std::string& s, char x = '#')
{
  s.erase(std::remove_if(s.begin(), s.end(), [x](char c) { return c == x; }), s.end());
}

Token ParseToken(const std::string& s)
{

        auto delim = s.find(' ');
  const auto type  = GetType(s.substr(0, delim));
        auto value = s.substr(delim + 1);
  strip_char(value);
  return Token{type, value};
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

std::vector<Token> GetTokens(const std::string& s)
{
  return conversation::SplitTokens(conversation::TokenizeText(s));
}

/**
 * DetectQuestionType
 *
 * @param
 * @returns
 */
Probe DetectProbeType(const std::string& text)
{
  std::string s = text;
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  const auto size = s.size();
  for (int i = 0; i < PRTypeNames.size(); i++)
    if (const auto idx = s.find(PRTypeNames[i]); idx != std::string::npos)
    {
      auto p_sz     = PRTypeNames.at(i).size();
      auto left_ok  = (idx == 2        || idx > 2 && (s.at(idx - 1   ) == ' ' || s.at(idx - 1   ) == '.'));
      auto right_ok = (idx == size - 1 ||            (s.at(idx + p_sz) == ' ' || s.at(idx + p_sz) == '.'));
      if (left_ok && right_ok)
        return static_cast<conversation::Probe::Type>(i);
    }
  return conversation::Probe::Type::UNKNOWN;
}

/**
 * Is Question
 */
size_t IsQuestion(const std::string& s)
{
  return s.find("?");
}

bool IsContinuing(Message* node) {
  while (node->next)
  {
    if (node->next->received == false)
      return true;
    node = node->next;
  }
  return false;
}

bool IsAssertion(const std::string& s)
{
  return false;
}

std::string FindImperative(const std::string& s)
{
  if (s.empty())
    return s;

  std::string lower_s = s;
  std::transform(lower_s.begin(), lower_s.end(), lower_s.begin(), ::tolower);

  for (const auto& imp_w : ImperativeNames)
    if (lower_s.find(imp_w) != lower_s.npos)
      return imp_w;
  return "";
}

bool IsSinglePhrase(const std::string& s)
{
  if (s.empty())
    return false;


  auto find_next_phrase_by_dot = [&s]
  {
    auto pt_idx = s.find('.');
    if (pt_idx == s.npos)                                // None
      return false;

    if (pt_idx != 0)
    {
      if (auto title = is_title(s.cbegin(), pt_idx)) // Possibly `Mr.` `Mrs.`
        while (title)
        {
          pt_idx = s.find('.', ++pt_idx);

          if (pt_idx == s.npos)
            return false;                               //
          title = is_title(s.cbegin(), pt_idx);
        }
      else
        return true;
    }

    while (pt_idx != s.npos && is_soft_stop(s.substr(pt_idx + 1), s.at(pt_idx)))
      pt_idx = s.find(++pt_idx, '.');

    pt_idx = s.find(++pt_idx, '.');
    return (pt_idx == s.npos || pt_idx == s.size());
  };

  auto find_by_break = [&s]
  {
    auto pt_idx = s.find('\n');
    // if (pt_idx == s.npos)
    //   return true;
    return false;
  };

  const auto has_multiple_by_dot = find_next_phrase_by_dot();

  if (has_multiple_by_dot)
    return false;

  // if (find_by_break())

  return true;
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
                    {"x-rapidapi-key" , API_KEY}},
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
          .word  = item["word"].get<std::string>(),
          .score = item["score"].get<float>()});
    }
  }
  else
    log("GetSentiment request failed.", response.GetError().c_str());

  return sentiment;
}

/**
 *
 */
Emotion GetEmotion(const std::string& query)
{
  Emotion emotion;
  RequestResponse response{cpr::Post(
    cpr::Url       {URLS[EMOTION_ANALYZER_INDEX]},
    cpr::Header    {{"x-rapidapi-host", "twinword-emotion-analysis-v1.p.rapidapi.com"},
                    {"x-rapidapi-key" , API_KEY}},
    cpr::Parameters{{"text", query}})};

  if (!response.error)
    emotion = Emotion::Parse(response.json());
  else
    log("GetEmotion request failed.", response.GetError().c_str());

  return emotion;
}

/**
 * @constructor
 */
NLP::NLP(const std::string& username)
: m_username(username)
{}

/**
 *
 */
Message* NLP::Insert(Message&& node, const std::string& name)
{
  m_q.emplace_back(std::move(node));
  Message*                  node_ref = &m_q.back();
  const auto&               tokens   = node_ref->tokens;
  const auto                subject  = (tokens.size()) ? tokens.front().value : "unknown";
  const Map::const_iterator it       = m_m.find(name);

  if (it == m_m.end())                                       // New
  {
    m_o.emplace_back(std::move(ObjectiveContext { node_ref }));
    m_s.emplace_back(std::move(SubjectiveContext{ subject  }));

    ObjectiveContext*  o_ctx_ref = &m_o.back();
    SubjectiveContext* s_ctx_ref = &m_s.back();

    node_ref->objective          = o_ctx_ref;
    node_ref->subjective         = s_ctx_ref;
    node_ref->next               = nullptr;

    m_m.insert({ name, node_ref });
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
  }

  SetContext(node_ref, tokens);

  return node_ref;
}


/**
 * Reply
 */
void NLP::Reply(Message* node, std::string reply, std::string name) {
  Message reply_node{
    reply,
    false,
    node->next,
    node->subjective,
    node->objective,
    {}};

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

    node_string +=
"\n│Interlocutor: " + interlocutor +
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
  using args_t = std::vector<std::string>;

  auto analyze_phrase = [this, &node](const auto& text, ObjectiveContext& o_ctx, SubjectiveContext& s_ctx)
  {
    o_ctx.imperative_word = FindImperative(text);
    o_ctx.is_imperative   = !o_ctx.imperative_word.empty();
    o_ctx.is_assertion    = !o_ctx.is_imperative && !o_ctx.is_question;
    const auto probe      = conversation::PRTypeNames.at(o_ctx.probe_type.to_int());
    const auto p_idx      = text.find(probe);
          auto subject    = node->find_subject((p_idx != text.npos) ? text.substr(p_idx) : text);
    if (subject.empty() && (p_idx != text.npos))
      subject = node->find_subject(text.substr(0, p_idx + probe.size()));
    return subject;
  };

  try
  {
    SubjectiveContext subjective_ctx{tokens};
    ObjectiveContext  objective_ctx {node};

    m_o.emplace_back(std::move(objective_ctx));
    m_s.emplace_back(std::move(subjective_ctx));

    node->objective  = &m_o.back();
    node->subjective = &m_s.back();

    auto& o_ctx      = *(node->objective);
    auto& s_ctx      = *(node->subjective);

    o_ctx.is_continuing    = IsContinuing(node);
    o_ctx.probe_type       = DetectProbeType(node->text);
    o_ctx.q_index          = IsQuestion(node->text);
    o_ctx.is_single_phrase = IsSinglePhrase(node->text);
    o_ctx.is_question      = o_ctx.q_index != std::string::npos;

    for (const auto& phrase : o_ctx.is_single_phrase ? args_t{node->text} : Split(node->text, '.'))
      s_ctx.Insert(analyze_phrase(phrase, o_ctx, s_ctx));

    if (!o_ctx.is_single_phrase)
      for (const auto& phrase : extract_phrases(node->text))
        node->expand(std::move(*Insert(Message{phrase, false, nullptr, nullptr, nullptr, GetTokens(phrase)}, "")));
  }
  catch (const std::exception& e)
  {
    std::cout << "Exception caught: " << e.what() << std::endl;
    return false;
  }

  return true;
}

std::string
Message::find_subject(const std::string& s) const
{
  if (s.empty())
    return s;

  auto pt_idx = s.find_last_of('.');
  if (!objective->is_single_phrase && pt_idx != s.npos && pt_idx != s.size())
  {
    auto nib1 = s.substr(0, pt_idx);
    auto nib2 = s.substr(pt_idx + 1);

    auto subject1 = find_subject(nib1);
    auto subject2 = find_subject(nib2);
    if (!subject1.empty())
      return subject1;
    if (!subject2.empty())
      return subject2;
  }

  if (objective->is_assertion && objective->probe_type == Probe::Type::IS)
  {

    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (const auto pr_idx = lower.find("is"); pr_idx)
    {
      int start = pr_idx - 2;
      for (int i = start; i > 0; i--)
      {
        if ((i != start) && !(std::isalnum(static_cast<int>(s.at(i)))))
        {
          int j;
          for (j = i; std::isalnum(s.at(j)) && j < s.size(); j++)
            ;

          if (std::string ret = s.substr(i, j - i); !ret.empty())
          {
            if (!std::isalnum(ret.front()))
              ret.erase(0, 1);
            if (!std::isalnum(ret.back()))
              ret.pop_back();
            return ret;
          }
        }
        else
        if (i == start && i == 0)
          break;
      }
    }
  }
  return "";
}
} // namespace conversation
