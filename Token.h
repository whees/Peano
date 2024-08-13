#ifndef TOKEN_H
#define TOKEN_H
#define END '\0'
#define NL '\n'
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
enum class types {
	ADD,
	ASSIGN,
	BASE,
	DEF,
	DIV,
	EoF,
	GET,
	GETN,
	LBRACK, RBRACK,
	LPAREN, RPAREN,
	MUL,
	NAME,
	NUM,
	NONE,
	PIPE,
	POW,
	PROGRAM,
	SEMI,
	SET,
	SHOW,
	SUB,
	SUCC,
	SELF,
	TYPE
};

struct Token {
	Token() : type{ types::NONE } {}
	Token(types type_, string value_) : type{ type_ }, value{ value_ } {}
	~Token() {}

	types type;
	string value;
};

extern unordered_map<string, Token> RESERVED_KEYWORDS;
#endif

