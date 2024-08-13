#include "Token.h"
unordered_map<string, Token> RESERVED_KEYWORDS = { {"show", Token(types::SHOW,"show")},
													{"int", Token(types::TYPE,"int")},
													{"float", Token(types::TYPE,"float")},
													{"uint", Token(types::TYPE,"uint")} };





