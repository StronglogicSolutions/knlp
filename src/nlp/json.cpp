#include "nlp.hpp"

namespace conversation {

nlohmann::json
TokensToJSON(const std::vector<Token>& tokens)
{
  nlohmann::json data = nlohmann::json::array();

  for (const auto& token : tokens)
  {
    nlohmann::json json{};
    json["type"] = TOKEN_TYPES[token.type];
    json["value"] = token.value;
    data.emplace_back(json);
  }
  return data;
}


} // ns conversation
