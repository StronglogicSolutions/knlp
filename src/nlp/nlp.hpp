#pragma once

#include <string>
#include <string_view>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "types.hpp"


namespace conversation
{
std::string        TokenizeText(std::string s);
std::vector<Token> SplitTokens(std::string s);
Token              ParseToken(std::string s);
TokenType          GetType(std::string type);
ProbeType          DetectProbeType(std::string s);
bool               IsQuestion(std::string s);

class NLP
{
public:
NLP(std::string username)
: m_username{username} {}

Message*         Insert(Message&& node, std::string name, std::string subject);
void             Reply(Message* node, std::string reply, std::string name);
bool             SetContext(Message* node);
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