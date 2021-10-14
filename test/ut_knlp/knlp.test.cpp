#include "knlp.test.hpp"

std::vector<std::string> GetReplyMessage(const std::string& s, conversation::Tokens tokens)
{

  std::vector<std::string> reply_messages{"I hear you"};
  // if (bot_was_mentioned) {
  auto message = s;

  return reply_messages;
}

TEST(KNLPTests, DISABLED_Converse)
{
  using Tokens = std::vector<conversation::Token>;
  static const std::string END_WORD{"exit"};
  static const std::string USERNAME{"logicp"};
  std::string              line{};
  conversation::NLP        nlp{USERNAME};

  for(;;)
  {
    std::getline(std::cin, line);
    if (line == END_WORD) break;

    conversation::ObjectiveContext objective_ctx = conversation::ObjectiveContext::Create(line);
    conversation::Tokens           tokens        = conversation::SplitTokens(conversation::TokenizeText(line));
    conversation::Message          message        {.text = line, .received = false};
    const auto                     replies       = GetReplyMessage(message.text, tokens);
    conversation::Message          rep_msg        {.text=replies.front(), .received = true};
    conversation::Message*         msg_ptr       = nlp.Insert(std::move(message), USERNAME, (!tokens.empty()) ? tokens.front().value : "unknown");
    conversation::Message*         rep_ptr       = nlp.Insert(std::move(rep_msg), USERNAME, "unknown");
    nlp.SetContext(msg_ptr);
  }

  const auto final_s = nlp.toString();
  std::cout << final_s << std::endl;

  EXPECT_FALSE(final_s.empty());
}

TEST(KNLPTests, SentimentAnalyzerTest)
{
  static const std::string query{"There once was not a man from Nantucket, whose chopsticks were not long enough to eat with"};

  conversation::Sentiment sentiment = conversation::GetSentiment(query);
  for (const auto& keyword : sentiment.keywords)
    conversation::log(keyword.word, std::string{" has a score of "}, std::to_string(keyword.score));

  EXPECT_TRUE(sentiment.score > 0.0f);
}

