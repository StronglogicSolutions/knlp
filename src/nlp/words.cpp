#include "nlp.hpp"

namespace conversation
{
const word_map_t g_verb_map{
  { "be", true },
  { "have", true },
  { "do", true },
  { "say", true },
  { "go", true },
  { "can", true },
  { "get", true },
  { "would", true },
  { "make", true },
  { "know", true },
  { "will", true },
  { "think", true },
  { "take", true },
  { "see", true },
  { "come", true },
  { "could", true },
  { "want", true },
  { "look", true },
  { "use", true },
  { "find", true },
  { "give", true },
  { "tell", true },
  { "work", true },
  { "may", true },
  { "should", true },
  { "call", true },
  { "try", true },
  { "ask", true },
  { "need", true },
  { "feel", true },
  { "become", true },
  { "leave", true },
  { "put", true },
  { "mean", true },
  { "keep", true },
  { "let", true },
  { "begin", true },
  { "seem", true },
  { "help", true },
  { "talk", true },
  { "turn", true },
  { "start", true },
  { "might", true },
  { "show", true },
  { "hear", true },
  { "play", true },
  { "run", true },
  { "move", true },
  { "like", true },
  { "live", true },
  { "believe", true },
  { "hold", true },
  { "bring", true },
  { "happen", true },
  { "must", true },
  { "write", true },
  { "provide", true },
  { "sit", true },
  { "stand", true },
  { "lose", true },
  { "pay", true },
  { "meet", true },
  { "include", true },
  { "continue", true },
  { "set", true },
  { "learn", true },
  { "change", true },
  { "lead", true },
  { "understand", true },
  { "watch", true },
  { "follow", true },
  { "stop", true },
  { "create", true },
  { "speak", true },
  { "read", true },
  { "allow", true },
  { "add", true },
  { "spend", true },
  { "grow", true },
  { "open", true },
  { "walk", true },
  { "win", true },
  { "offer", true },
  { "remember", true },
  { "love", true },
  { "consider", true },
  { "appear", true },
  { "buy", true },
  { "wait", true },
  { "serve", true },
  { "die", true },
  { "send", true },
  { "expect", true },
  { "build", true },
  { "stay", true },
  { "fall", true },
  { "cut", true },
  { "reach", true },
  { "kill", true },
  { "remain", true }
};

const word_map_t g_prep_map{
{ "aboard", true },
{ "about", true },
{ "above", true },
{ "across", true },
{ "after", true },
{ "against", true },
{ "along", true },
{ "amid", true },
{ "among", true },
{ "around", true },
{ "as", true },
{ "at", true },
{ "before", true },
{ "behind", true },
{ "below", true },
{ "beneath", true },
{ "beside", true },
{ "between", true },
{ "beyond", true },
{ "but", true },
{ "by", true },
{ "concerning", true },
{ "considering", true },
{ "despite", true },
{ "down", true },
{ "during", true },
{ "except", true },
{ "following", true },
{ "for", true },
{ "from", true },
{ "in", true },
{ "inside", true },
{ "into", true },
{ "like", true },
{ "minus", true },
{ "near", true },
{ "next", true },
{ "of", true },
{ "off", true },
{ "on", true },
{ "onto", true },
{ "opposite", true },
{ "out", true },
{ "outside", true },
{ "over", true },
{ "past", true },
{ "per", true },
{ "plus", true },
{ "regarding", true },
{ "round", true },
{ "save", true },
{ "since", true },
{ "than", true },
{ "through", true },
{ "till", true },
{ "to", true },
{ "toward", true },
{ "under", true },
{ "underneath", true },
{ "unlike", true },
{ "until", true },
{ "up", true },
{ "upon", true },
{ "versus", true },
{ "via", true },
{ "with", true },
{ "within", true },
{ "without", true },
};
} // ns conversation