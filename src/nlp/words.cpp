#include "nlp.hpp"

namespace conversation
{
enum class conjug_t
{
  present, past, future
};

std::string
get_tense(conjug_t conjugation)
{
  if (conjugation == conjug_t::present)
    return "present";
  if (conjugation == conjug_t::past)
    return "past";
  if (conjugation == conjug_t::future)
    return "future";
}

using verb_map_t = std::map<std::string, conjug_t>;
class verb
{
public:
  verb(verb_map_t map)
  : map_(map),
    root_(present())
  {}

  std::string present() const
  {
    for (const auto& [word, conjugation] : map_)
      if (conjugation == conjug_t::present)
        return word;
    return "";
  }
  std::string future() const
  {
    for (const auto& [word, conjugation] : map_)

      if (conjugation == conjug_t::future)
        return word;
    return "";
  }
  std::string past() const
  {
    for (const auto& [word, conjugation] : map_)
      if (conjugation == conjug_t::past)
        return word;
    return "";
  }

  std::string tense(const std::string& v) const
  {
    for (const auto& [word, conjugation] : map_)
      if (word == v)
        return get_tense(conjugation);
    return "";
  }

protected:
  verb_map_t  map_;
  std::string root_;
};
verb_map_t am_map{
  {"am", conjug_t::present}
};
using vm_t = verb_map_t;
std::array<verb, 106> g_verbs{
  vm_t{
    {"am",              conjug_t::present},
    {"will be",         conjug_t::future},
    {"was",             conjug_t::past}
  },
  vm_t{
    {"is",              conjug_t::present},
    {"will be",         conjug_t::future},
    {"was",             conjug_t::past}
  },
  vm_t{
    {"have",            conjug_t::present},
    {"will have",       conjug_t::future},
    {"had",             conjug_t::past}
  },
  vm_t{
    {"do",              conjug_t::present},
    {"will do",         conjug_t::future},
    {"did",             conjug_t::past}
  },
  vm_t{
    {"say",             conjug_t::present},
    {"will say",        conjug_t::future},
    {"said",            conjug_t::past},
  },
  vm_t{
    {"can",             conjug_t::present},
    {"will be able to", conjug_t::future},
    {"could",           conjug_t::past},
  },
  vm_t{
    {"go",              conjug_t::present},
    {"will go",         conjug_t::future},
    {"went",            conjug_t::past},
  },
  vm_t{
    {"get",             conjug_t::present},
    {"will get",        conjug_t::future},
    {"got",             conjug_t::past},
  },
  vm_t{
    {"would",           conjug_t::present},
    {"would",           conjug_t::future},
    {"would",           conjug_t::past},
  },
  vm_t{
    {"make",            conjug_t::present},
    {"will make",       conjug_t::future},
    {"made",            conjug_t::past},
  },
  vm_t{
    {"know",            conjug_t::present},
    {"will know",       conjug_t::future},
    {"knew",            conjug_t::past},
  },
  vm_t{
    {"think",           conjug_t::present},
    {"will think",      conjug_t::future},
    {"thought",         conjug_t::past},
  },
  vm_t{
    {"take",            conjug_t::present},
    {"will take",       conjug_t::future},
    {"took",            conjug_t::past},
  },
  vm_t{
    {"see",             conjug_t::present},
    {"will see",        conjug_t::future},
    {"saw",             conjug_t::past},
  },
  vm_t{
    {"come",            conjug_t::present},
    {"will come",       conjug_t::future},
    {"came",            conjug_t::past},
  },
  vm_t{
    {"can",             conjug_t::present},
    {"would",           conjug_t::future},
    {"would",           conjug_t::past},
  },
  vm_t{
    {"want",            conjug_t::present},
    {"will want",       conjug_t::future},
    {"wanted",          conjug_t::past},
  },
  vm_t{
    {"look",            conjug_t::present},
    {"will look",       conjug_t::future},
    {"looked",          conjug_t::past}
  },
  vm_t{
    {"use",             conjug_t::present},
    {"will use",        conjug_t::future},
    {"used",            conjug_t::past},
  },
  vm_t{
    {"find",            conjug_t::present},
    {"will find",       conjug_t::future},
    {"found",           conjug_t::past},
  },
  vm_t{
    {"give",            conjug_t::present},
    {"will give",       conjug_t::future},
    {"gave",            conjug_t::past}
    },
  vm_t{
    {"tell",            conjug_t::present},
    {"will tell",       conjug_t::future},
    {"told",            conjug_t::past}
    },
  vm_t{
    {"work",            conjug_t::present},
    {"will work",       conjug_t::future},
    {"worked",          conjug_t::past}
    },
  vm_t{
    {"may",             conjug_t::present},
    {"may",             conjug_t::future},
    {"might",           conjug_t::past}
    },
  vm_t{
    {"should", conjug_t::present},
    {"should", conjug_t::future},
    {"should", conjug_t::past}
    },
  vm_t{
    {"call", conjug_t::present},
    {"will call", conjug_t::future},
    {"called", conjug_t::past}
    },
  vm_t{
    {"try", conjug_t::present},
    {"will try", conjug_t::future},
    {"tried", conjug_t::past}
    },
  vm_t{
    {"ask", conjug_t::present},
    {"will ask", conjug_t::future},
    {"asked", conjug_t::past}
    },
  vm_t{
    {"need", conjug_t::present},
    {"will need", conjug_t::future},
    {"needed", conjug_t::past}
    },
  vm_t{
    {"feel", conjug_t::present},
    {"will feel", conjug_t::future},
    {"felt", conjug_t::past}
    },
  vm_t{
    {"become", conjug_t::present},
    {"will become", conjug_t::future},
    {"became", conjug_t::past}
    },
  vm_t{
    {"leave", conjug_t::present},
    {"will leave", conjug_t::future},
    {"left", conjug_t::past}
    },
  vm_t{
    {"put", conjug_t::present},
    {"will put", conjug_t::future},
    {"put", conjug_t::past}
    },
  vm_t{
    {"mean", conjug_t::present},
    {"will mean", conjug_t::future},
    {"meant", conjug_t::past}
    },
  vm_t{
    {"keep", conjug_t::present},
    {"will keep", conjug_t::future},
    {"kept", conjug_t::past}
    },
  vm_t{
    {"let", conjug_t::present},
    {"will let", conjug_t::future},
    {"let", conjug_t::past}
    },
  vm_t{
    {"begin", conjug_t::present},
    {"will begin", conjug_t::future},
    {"began", conjug_t::past}
    },
  vm_t{
    {"seem", conjug_t::present},
    {"will seem", conjug_t::future},
    {"seemed", conjug_t::past}
    },
  vm_t{
    {"help", conjug_t::present},
    {"will help", conjug_t::future},
    {"helped", conjug_t::past}
    },
  vm_t{
    {"talk", conjug_t::present},
    {"will talk", conjug_t::future},
    {"talked", conjug_t::past}
    },
  vm_t{
    {"turn", conjug_t::present},
    {"will turn", conjug_t::future},
    {"turned", conjug_t::past}
    },
  vm_t{
    {"start", conjug_t::present},
    {"will start", conjug_t::future},
    {"started", conjug_t::past}
    },
  vm_t{
    {"show", conjug_t::present},
    {"will show", conjug_t::future},
    {"showed", conjug_t::past}
    },
  vm_t{
    {"hear", conjug_t::present},
    {"will hear", conjug_t::future},
    {"heard", conjug_t::past}
    },
  vm_t{
    {"play", conjug_t::present},
    {"will play", conjug_t::future},
    {"played", conjug_t::past}
    },
  vm_t{
    {"run", conjug_t::present},
    {"will run", conjug_t::future},
    {"ran", conjug_t::past}
    },
  vm_t{
    {"move", conjug_t::present},
    {"will move", conjug_t::future},
    {"moved", conjug_t::past}
    },
  vm_t{
    {"like", conjug_t::present},
    {"will like", conjug_t::future},
    {"liked", conjug_t::past}
    },
  vm_t{
    {"live", conjug_t::present},
    {"will live", conjug_t::future},
    {"lived", conjug_t::past}
    },
  vm_t{
    {"believe", conjug_t::present},
    {"will believe", conjug_t::future},
    {"believed", conjug_t::past}
    },
  vm_t{
    {"hold", conjug_t::present},
    {"will hold", conjug_t::future},
    {"held", conjug_t::past}
    },
  vm_t{
    {"bring", conjug_t::present},
    {"will bring", conjug_t::future},
    {"brought", conjug_t::past}
    },
  vm_t{
    {"happen", conjug_t::present},
    {"will happen", conjug_t::future},
    {"happened", conjug_t::past}
    },
  vm_t{
    {"must", conjug_t::present},
    {"will have to", conjug_t::future},
    {"must", conjug_t::past}
    },
  vm_t{
    {"write", conjug_t::present},
    {"will write", conjug_t::future},
    {"wrote", conjug_t::past}
    },
  vm_t{
    {"provide", conjug_t::present},
    {"will provide", conjug_t::future},
    {"provided", conjug_t::past}
    },
  vm_t{
    {"sit", conjug_t::present},
    {"will sit", conjug_t::future},
    {"sat", conjug_t::past}
    },
  vm_t{
    {"stand", conjug_t::present},
    {"will stand", conjug_t::future},
    {"stood", conjug_t::past}
    },
  vm_t{
    {"lose", conjug_t::present},
    {"will lose", conjug_t::future},
    {"lost", conjug_t::past}
    },
  vm_t{
    {"pay", conjug_t::present},
    {"will pay", conjug_t::future},
    {"payed", conjug_t::past}
    },
  vm_t{
    {"meet", conjug_t::present},
    {"will meet", conjug_t::future},
    {"met", conjug_t::past}
    },
  vm_t{
    {"include", conjug_t::present},
    {"will include", conjug_t::future},
    {"included", conjug_t::past}
    },
  vm_t{
    {"continue", conjug_t::present},
    {"will continue", conjug_t::future},
    {"continued", conjug_t::past}
    },
  vm_t{
    {"set", conjug_t::present},
    {"will set", conjug_t::future},
    {"set", conjug_t::past}
    },
  vm_t{
    {"learn", conjug_t::present},
    {"will learn", conjug_t::future},
    {"learnt", conjug_t::past}
    },
  vm_t{
    {"change", conjug_t::present},
    {"will change", conjug_t::future},
    {"changed", conjug_t::past}
    },
  vm_t{
    {"lead", conjug_t::present},
    {"will lead", conjug_t::future},
    {"led", conjug_t::past}
    },
  vm_t{
    {"understand", conjug_t::present},
    {"will understand", conjug_t::future},
    {"understood", conjug_t::past}
    },
  vm_t{
    {"watch", conjug_t::present},
    {"will watch", conjug_t::future},
    {"watched", conjug_t::past}
    },
  vm_t{
    {"follow", conjug_t::present},
    {"will follow", conjug_t::future},
    {"follow", conjug_t::past}
    },
  vm_t{
    {"stop", conjug_t::present},
    {"will stop", conjug_t::future},
    {"stopped", conjug_t::past}
    },
  vm_t{
    {"create", conjug_t::present},
    {"will create", conjug_t::future},
    {"created", conjug_t::past}
    },
  vm_t{
    {"speak", conjug_t::present},
    {"will speak", conjug_t::future},
    {"spoke", conjug_t::past}
    },
  vm_t{
    {"read", conjug_t::present},
    {"will read", conjug_t::future},
    {"read", conjug_t::past}
    },
  vm_t{
    {"allow", conjug_t::present},
    {"will allow", conjug_t::future},
    {"allowed", conjug_t::past}
    },
  vm_t{
    {"add", conjug_t::present},
    {"will add", conjug_t::future},
    {"added", conjug_t::past}
    },
  vm_t{
    {"spend", conjug_t::present},
    {"will spend", conjug_t::future},
    {"spent", conjug_t::past}
    },
  vm_t{
    {"grow", conjug_t::present},
    {"will grow", conjug_t::future},
    {"grew", conjug_t::past}
    },
  vm_t{
    {"open", conjug_t::present},
    {"will open", conjug_t::future},
    {"opened", conjug_t::past}
    },
  vm_t{
    {"walk", conjug_t::present},
    {"will walk", conjug_t::future},
    {"walked", conjug_t::past}
    },
  vm_t{
    {"win", conjug_t::present},
    {"will win", conjug_t::future},
    {"won", conjug_t::past}
    },
  vm_t{
    {"offer", conjug_t::present},
    {"will offer", conjug_t::future},
    {"offered", conjug_t::past}
    },
  vm_t{
    {"remember", conjug_t::present},
    {"will remember", conjug_t::future},
    {"remembered", conjug_t::past}
    },
  vm_t{
    {"love", conjug_t::present},
    {"will love", conjug_t::future},
    {"loved", conjug_t::past}
    },
  vm_t{
    {"consider", conjug_t::present},
    {"will consider", conjug_t::future},
    {"considered", conjug_t::past}
    },
  vm_t{
    {"appear", conjug_t::present},
    {"will appear", conjug_t::future},
    {"appeared", conjug_t::past}
    },
  vm_t{
    {"buy", conjug_t::present},
    {"will buy", conjug_t::future},
    {"bought", conjug_t::past}
    },
  vm_t{
    {"wait", conjug_t::present},
    {"will wait", conjug_t::future},
    {"waited", conjug_t::past}
    },
  vm_t{
    {"serve", conjug_t::present},
    {"will serve", conjug_t::future},
    {"served", conjug_t::past}
    },
  vm_t{
    {"die", conjug_t::present},
    {"will die", conjug_t::future},
    {"died", conjug_t::past}
    },
  vm_t{
    {"send", conjug_t::present},
    {"will send", conjug_t::future},
    {"sent", conjug_t::past}
    },
  vm_t{
    {"expect", conjug_t::present},
    {"will expect", conjug_t::future},
    {"expected", conjug_t::past}
    },
  vm_t{
    {"build", conjug_t::present},
    {"will build", conjug_t::future},
    {"built", conjug_t::past}
    },
  vm_t{
    {"stay", conjug_t::present},
    {"will stay", conjug_t::future},
    {"stayed", conjug_t::past}
    },
  vm_t{
    {"fall", conjug_t::present},
    {"will fall", conjug_t::future},
    {"fell", conjug_t::past}
    },
  vm_t{
    {"cut", conjug_t::present},
    {"will cut", conjug_t::future},
    {"cut", conjug_t::past}
    },
  vm_t{
    {"check", conjug_t::present},
    {"will check", conjug_t::future},
    {"checked", conjug_t::past}
    },
  vm_t{
    {"reach", conjug_t::present},
    {"will reach", conjug_t::future},
    {"reached", conjug_t::past}
    },
  vm_t{
    {"kill", conjug_t::present},
    {"will kill", conjug_t::future},
    {"killed", conjug_t::past}
    },
  vm_t{
    {"remain", conjug_t::present},
    {"will remain", conjug_t::future},
    {"remained", conjug_t::past}
    },
  vm_t{
    {"listen", conjug_t::present},
    {"will listen", conjug_t::future},
    {"listened", conjug_t::past}
    },
  vm_t{
    {"write", conjug_t::present},
    {"will write", conjug_t::future},
    {"wrote", conjug_t::past}
    },
  vm_t{
    {"prevent", conjug_t::present},
    {"will prevent", conjug_t::future},
    {"prevented", conjug_t::past}
    },
  vm_t{
    {"bypass",         conjug_t::present},
    {"will bypass",    conjug_t::future},
    {"bypassed",       conjug_t::past}
    },
  vm_t{
    {"advocate",       conjug_t::present},
    {"will advocate",  conjug_t::future},
    {"advocated",      conjug_t::past}
    },
  vm_t{
    {"discuss",        conjug_t::present},
    {"will discuss",   conjug_t::future},
    {"discussed",      conjug_t::past}
    },
};

const word_map_t g_verb_map{
{ "am",              &g_verbs[0] },
{ "will be",         &g_verbs[0] },
{ "was",             &g_verbs[0] },
{ "is",              &g_verbs[1] },
{ "will be",         &g_verbs[1] },
{ "was",             &g_verbs[1] },
{ "have",            &g_verbs[2] },
{ "will have",       &g_verbs[2] },
{ "had",             &g_verbs[2] },
{ "do",              &g_verbs[3] },
{ "will do",         &g_verbs[3] },
{ "did",             &g_verbs[3] },
{ "say",             &g_verbs[4] },
{ "will say",        &g_verbs[4] },
{ "said",            &g_verbs[4] },
{ "can",             &g_verbs[5] },
{ "will be able to", &g_verbs[5] },
{ "could",           &g_verbs[5] },
{ "go",              &g_verbs[6] },
{ "will go",         &g_verbs[6] },
{ "went",            &g_verbs[6] },
{ "get",             &g_verbs[7] },
{ "will get",        &g_verbs[7] },
{ "got",             &g_verbs[7] },
{ "would",           &g_verbs[8] },
{ "would",           &g_verbs[8] },
{ "would",           &g_verbs[8] },
{ "make",            &g_verbs[9] },
{ "will make",       &g_verbs[9] },
{ "made",            &g_verbs[9] },
{ "know",            &g_verbs[10] },
{ "will know",       &g_verbs[10] },
{ "knew",            &g_verbs[10] },
{ "think",           &g_verbs[11] },
{ "will think",      &g_verbs[11] },
{ "thought",         &g_verbs[11] },
{ "take",            &g_verbs[12] },
{ "will take",       &g_verbs[12] },
{ "took",            &g_verbs[12] },
{ "see",             &g_verbs[13] },
{ "will see",        &g_verbs[13] },
{ "saw",             &g_verbs[13] },
{ "come",            &g_verbs[14] },
{ "will come",       &g_verbs[14] },
{ "came",            &g_verbs[14] },
{ "can",             &g_verbs[15] },
{ "would",           &g_verbs[15] },
{ "would",           &g_verbs[15] },
{ "want",            &g_verbs[16] },
{ "will want",       &g_verbs[16] },
{ "wanted",          &g_verbs[16] },
{ "look",            &g_verbs[17] },
{ "will look",       &g_verbs[17] },
{ "looked",          &g_verbs[17] },
{ "use",             &g_verbs[18] },
{ "will use",        &g_verbs[18] },
{ "used",            &g_verbs[18] },
{ "find",            &g_verbs[19] },
{ "will find",       &g_verbs[19] },
{ "found",           &g_verbs[19] },
{ "give",            &g_verbs[20] },
{ "will give",       &g_verbs[20] },
{ "gave",            &g_verbs[20] },
{ "tell",            &g_verbs[21] },
{ "will tell",       &g_verbs[21] },
{ "told",            &g_verbs[21] },
{ "work",            &g_verbs[22] },
{ "will work",       &g_verbs[22] },
{ "worked",          &g_verbs[22] },
{ "may",             &g_verbs[23] },
{ "may",             &g_verbs[23] },
{ "might",           &g_verbs[23] },
{ "should",          &g_verbs[24] },
{ "should",          &g_verbs[24] },
{ "should",          &g_verbs[24] },
{ "call",            &g_verbs[25] },
{ "will call",       &g_verbs[25] },
{ "called",          &g_verbs[25] },
{ "try",             &g_verbs[26] },
{ "will try",        &g_verbs[26] },
{ "tried",           &g_verbs[26] },
{ "ask",             &g_verbs[27] },
{ "will ask",        &g_verbs[27] },
{ "asked",           &g_verbs[27] },
{ "need",            &g_verbs[28] },
{ "will need",       &g_verbs[28] },
{ "needed",          &g_verbs[28] },
{ "feel",            &g_verbs[29] },
{ "will feel",       &g_verbs[29] },
{ "felt",            &g_verbs[29] },
{ "become",          &g_verbs[30] },
{ "will become",     &g_verbs[30] },
{ "became",          &g_verbs[30] },
{ "leave",           &g_verbs[31] },
{ "will leave",      &g_verbs[31] },
{ "left",            &g_verbs[31] },
{ "put",             &g_verbs[32] },
{ "will put",        &g_verbs[32] },
{ "put",             &g_verbs[32] },
{ "mean",            &g_verbs[33] },
{ "will mean",       &g_verbs[33] },
{ "meant",           &g_verbs[33] },
{ "keep",            &g_verbs[34] },
{ "will keep",       &g_verbs[34] },
{ "kept",            &g_verbs[34] },
{ "let",             &g_verbs[35] },
{ "will let",        &g_verbs[35] },
{ "let",             &g_verbs[35] },
{ "begin",           &g_verbs[36] },
{ "will begin",      &g_verbs[36] },
{ "began",           &g_verbs[36] },
{ "seem",            &g_verbs[37] },
{ "will seem",       &g_verbs[37] },
{ "seemed",          &g_verbs[37] },
{ "help",            &g_verbs[38] },
{ "will help",       &g_verbs[38] },
{ "helped",          &g_verbs[38] },
{ "talk",            &g_verbs[39] },
{ "will talk",       &g_verbs[39] },
{ "talked",          &g_verbs[39] },
{ "turn",            &g_verbs[40] },
{ "will turn",       &g_verbs[40] },
{ "turned",          &g_verbs[40] },
{ "start",           &g_verbs[41] },
{ "will start",      &g_verbs[41] },
{ "started",         &g_verbs[41] },
{ "show",            &g_verbs[42] },
{ "will show",       &g_verbs[42] },
{ "showed",          &g_verbs[42] },
{ "hear",            &g_verbs[43] },
{ "will hear",       &g_verbs[43] },
{ "heard",           &g_verbs[43] },
{ "play",            &g_verbs[44] },
{ "will play",       &g_verbs[44] },
{ "played",          &g_verbs[44] },
{ "run",             &g_verbs[45] },
{ "will run",        &g_verbs[45] },
{ "ran",             &g_verbs[45] },
{ "move",            &g_verbs[46] },
{ "will move",       &g_verbs[46] },
{ "moved",           &g_verbs[46] },
{ "like",            &g_verbs[47] },
{ "will like",       &g_verbs[47] },
{ "liked",           &g_verbs[47] },
{ "live",            &g_verbs[48] },
{ "will live",       &g_verbs[48] },
{ "lived",           &g_verbs[48] },
{ "believe",         &g_verbs[49] },
{ "will believe",    &g_verbs[49] },
{ "believed",        &g_verbs[49] },
{ "hold",            &g_verbs[50] },
{ "will hold",       &g_verbs[50] },
{ "held",            &g_verbs[50] },
{ "bring",           &g_verbs[51] },
{ "will bring",      &g_verbs[51] },
{ "brought",         &g_verbs[51] },
{ "happen",          &g_verbs[52] },
{ "will happen",     &g_verbs[52] },
{ "happened",        &g_verbs[52] },
{ "must",            &g_verbs[53] },
{ "will have to",    &g_verbs[53] },
{ "must",            &g_verbs[53] },
{ "write",           &g_verbs[54] },
{ "will write",      &g_verbs[54] },
{ "wrote",           &g_verbs[54] },
{ "provide",         &g_verbs[55] },
{ "will provide",    &g_verbs[55] },
{ "provided",        &g_verbs[55] },
{ "sit",             &g_verbs[56] },
{ "will sit",        &g_verbs[56] },
{ "sat",             &g_verbs[56] },
{ "stand",           &g_verbs[57] },
{ "will stand",      &g_verbs[57] },
{ "stood",           &g_verbs[57] },
{ "lose",            &g_verbs[58] },
{ "will lose",       &g_verbs[58] },
{ "lost",            &g_verbs[58] },
{ "pay",             &g_verbs[59] },
{ "will pay",        &g_verbs[59] },
{ "payed",           &g_verbs[59] },
{ "meet",            &g_verbs[60] },
{ "will meet",       &g_verbs[60] },
{ "met",             &g_verbs[60] },
{ "include",         &g_verbs[61] },
{ "will include",    &g_verbs[61] },
{ "included",        &g_verbs[61] },
{ "continue",        &g_verbs[62] },
{ "will continue",   &g_verbs[62] },
{ "continued",       &g_verbs[62] },
{ "set",             &g_verbs[63] },
{ "will set",        &g_verbs[63] },
{ "set",             &g_verbs[63] },
{ "learn",           &g_verbs[64] },
{ "will learn",      &g_verbs[64] },
{ "learnt",          &g_verbs[64] },
{ "change",          &g_verbs[65] },
{ "will change",     &g_verbs[65] },
{ "changed",         &g_verbs[65] },
{ "lead",            &g_verbs[66] },
{ "will lead",       &g_verbs[66] },
{ "led",             &g_verbs[66] },
{ "understand",      &g_verbs[67] },
{ "will understand", &g_verbs[67] },
{ "understood",      &g_verbs[67] },
{ "watch",           &g_verbs[68] },
{ "will watch",      &g_verbs[68] },
{ "watched",         &g_verbs[68] },
{ "follow",          &g_verbs[69] },
{ "will follow",     &g_verbs[69] },
{ "follow",          &g_verbs[69] },
{ "stop",            &g_verbs[70] },
{ "will stop",       &g_verbs[70] },
{ "stopped",         &g_verbs[70] },
{ "create",          &g_verbs[71] },
{ "will create",     &g_verbs[71] },
{ "created",         &g_verbs[71] },
{ "speak",           &g_verbs[72] },
{ "will speak",      &g_verbs[72] },
{ "spoke",           &g_verbs[72] },
{ "read",            &g_verbs[73] },
{ "will read",       &g_verbs[73] },
{ "read",            &g_verbs[73] },
{ "allow",           &g_verbs[74] },
{ "will allow",      &g_verbs[74] },
{ "allowed",         &g_verbs[74] },
{ "add",             &g_verbs[75] },
{ "will add",        &g_verbs[75] },
{ "added",           &g_verbs[75] },
{ "spend",           &g_verbs[76] },
{ "will spend",      &g_verbs[76] },
{ "spent",           &g_verbs[76] },
{ "grow",            &g_verbs[77] },
{ "will grow",       &g_verbs[77] },
{ "grew",            &g_verbs[77] },
{ "open",            &g_verbs[78] },
{ "will open",       &g_verbs[78] },
{ "opened",          &g_verbs[78] },
{ "walk",            &g_verbs[79] },
{ "will walk",       &g_verbs[79] },
{ "walked",          &g_verbs[79] },
{ "win",             &g_verbs[80] },
{ "will win",        &g_verbs[80] },
{ "won",             &g_verbs[80] },
{ "offer",           &g_verbs[81] },
{ "will offer",      &g_verbs[81] },
{ "offered",         &g_verbs[81] },
{ "remember",        &g_verbs[82] },
{ "will remember",   &g_verbs[82] },
{ "remembered",      &g_verbs[82] },
{ "love",            &g_verbs[83] },
{ "will love",       &g_verbs[83] },
{ "loved",           &g_verbs[83] },
{ "consider",        &g_verbs[84] },
{ "will consider",   &g_verbs[84] },
{ "considered",      &g_verbs[84] },
{ "appear",          &g_verbs[85] },
{ "will appear",     &g_verbs[85] },
{ "appeared",        &g_verbs[85] },
{ "buy",             &g_verbs[86] },
{ "will buy",        &g_verbs[86] },
{ "bought",          &g_verbs[86] },
{ "wait",            &g_verbs[87] },
{ "will wait",       &g_verbs[87] },
{ "waited",          &g_verbs[87] },
{ "serve",           &g_verbs[88] },
{ "will serve",      &g_verbs[88] },
{ "served",          &g_verbs[88] },
{ "die",             &g_verbs[89] },
{ "will die",        &g_verbs[89] },
{ "died",            &g_verbs[89] },
{ "send",            &g_verbs[90] },
{ "will send",       &g_verbs[90] },
{ "sent",            &g_verbs[90] },
{ "expect",          &g_verbs[91] },
{ "will expect",     &g_verbs[91] },
{ "expected",        &g_verbs[91] },
{ "build",           &g_verbs[92] },
{ "will build",      &g_verbs[92] },
{ "built",           &g_verbs[92] },
{ "stay",            &g_verbs[93] },
{ "will stay",       &g_verbs[93] },
{ "stayed",          &g_verbs[93] },
{ "fall",            &g_verbs[94] },
{ "will fall",       &g_verbs[94] },
{ "fell",            &g_verbs[94] },
{ "cut",             &g_verbs[95] },
{ "will cut",        &g_verbs[95] },
{ "cut",             &g_verbs[95] },
{ "check",           &g_verbs[96] },
{ "will check",      &g_verbs[96] },
{ "checked",         &g_verbs[96] },
{ "reach",           &g_verbs[97] },
{ "will reach",      &g_verbs[97] },
{ "reached",         &g_verbs[97] },
{ "kill",            &g_verbs[98] },
{ "will kill",       &g_verbs[98] },
{ "killed",          &g_verbs[98] },
{ "remain",          &g_verbs[99] },
{ "will remain",     &g_verbs[99] },
{ "remained",        &g_verbs[99] },
{ "listen",          &g_verbs[100] },
{ "will listen",     &g_verbs[100] },
{ "listened",        &g_verbs[100] },
{ "write",           &g_verbs[101] },
{ "will write",      &g_verbs[101] },
{ "wrote",           &g_verbs[101] },
{ "prevent",         &g_verbs[102] },
{ "will prevent",    &g_verbs[102] },
{ "prevented",       &g_verbs[102] },
{ "bypass",          &g_verbs[103] },
{ "will bypass",     &g_verbs[103] },
{ "bypassed",        &g_verbs[103] },
{ "advocate",        &g_verbs[104] },
{ "will advocate",   &g_verbs[104] },
{ "advocated",       &g_verbs[104] },
{ "discuss",         &g_verbs[105] },
{ "will discuss",    &g_verbs[105] },
{ "discussed",       &g_verbs[105] }
};

const prep_map_t g_prep_map{
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