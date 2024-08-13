#ifndef PARSER_H
#define PARSER_H
#include "Lexer.h"
#include "Node.h"
#include "Parser.h"
#include <vector>
using namespace std;

struct Parser_Error {
	string what;

	Parser_Error(string what_) : what{ what_ } {}
	Parser_Error() : what{ "Parser error!" } {}
};

struct Parser {
	Lexer lexer;
	Token current_token;
	Node* tree;

	Parser() : lexer{ Lexer() }, current_token{ Token() }, tree{ nullptr } {}
	
	void load(Lexer lexer_);
	void eat(types type_);

	Node* program();
	vector<Node*> statement_list();
	Node* statement();
	Node* def_statement();
	Node* show_statement();
	Node* empty();
	Node* factor();
	Node* num();
	Node* name();
	Node* type();
	Node* set();
	vector<Node*> args();
	Node* expr();
	Node* term();
	Node* pow();
};
#endif

