#ifndef LEXER_H
#define LEXER_H
#include <ctype.h>
#include <iostream>
#include "Token.h"
#include "File.h"
#include <string>
using namespace std;

struct Lexer_Error {
	string what;
	Lexer_Error(string what_) : what{ what_ } {}
	Lexer_Error() : what{ "Lexer error!" } {}
};

struct Lexer {
	string buffer;
	char current_char;
	int pos;

	Lexer() : pos{ NULL }, current_char{ NULL } {}
	~Lexer() {}

	Token name();
	Token num();
	Token get_next_token();

	char peek();
	void advance();
	void skip_space();
	void load(File file_);
};
#endif
