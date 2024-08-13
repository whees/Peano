#include "Parser.h"

void Parser::load(Lexer lexer_) {
	try {
		lexer = lexer_;
		current_token = lexer.get_next_token();
		tree = program();
	}
	catch (Lexer_Error e) {
		throw Parser_Error(e.what);
	}
}

void Parser::eat(types type_) {
	if (current_token.type == type_)
		current_token = lexer.get_next_token();
	else
		throw Parser_Error("Invalid syntax eaten.");
}

Node* Parser::program() {
	vector<Node*> nodes = statement_list();
	Program* program = new Program();
	for (Node* node : nodes)
		program->append(node);
	
	return program;
}

vector<Node*> Parser::statement_list() {
	vector<Node*> statement_list;
	statement_list.push_back(statement());
	while (current_token.type == types::SEMI) {
		eat(types::SEMI);
		statement_list.push_back(statement());
	}

	return statement_list;
}

Node* Parser::statement() {
	switch (current_token.type) {
	case types::TYPE:
		return def_statement();
	case types::SHOW:
		return show_statement();
	default:
		return empty();
	}
}

Node* Parser::def_statement() {
	Node* type_ = type();
	Node* name_ = name();
	Node* set_ = set();
	Node* define_ = new Define(type_,name_,set_);
	return define_;
}

Node* Parser::show_statement() {
	eat(types::SHOW);
	Node* arg_ = factor();
	Node* show_ = new Show(arg_);
	return show_;
}

Node* Parser::name() {
	Node* name_ = new Name(current_token);
	eat(types::NAME);
	return name_;
}

Node* Parser::type() {
	Node* type_ = new Type(current_token);
	eat(types::TYPE);
	return type_;
}

Node* Parser::num() {
	Num* num_ = new Num(current_token);
	eat(types::NUM);
	return num_;
}

vector<Node*> Parser::args() {
	vector<Node*> args_;
	args_.push_back(expr());

	if (current_token.type == types::PIPE) {
		eat(types::PIPE);
		args_.push_back(expr());
	}

	return args_;
}

Node* Parser::set() {
	eat(types::LBRACK);
	vector<Node*> args_ = args();

	Node* base_ = new Num(Token(types::NUM, "0"));
	Node* succ_ = empty();
	if (args_.size() == 1) 
		succ_ = args_[0];
	else if (args_.size() == 2) {
		base_ = args_[0];
		succ_ = args_[1];
	}
	else
		throw Parser_Error("Invalid set argument count.");

	Node* set_ = new Set(base_,succ_);
	eat(types::RBRACK);
	return set_;
}

Node* Parser::empty() {
	return new NoOp;
}

Node* Parser::factor() {
	Token token = current_token;

	switch (token.type) {
	case types::NUM:
		return num();
	case types::NAME:
		return name();
	case types::LPAREN: {
		eat(types::LPAREN);
		Node* node = expr();
		eat(types::RPAREN);
		return node;
	}
	case types::GET: {
		Node* name_ = token.value == "()" ? new Self() : make_Name(token.value);
		eat(types::GET);
		return new Get(name_);
	}
	case types::SUCC: {
		Node* name_ = token.value == "(+)" ? new Self() : make_Name(token.value);
		eat(types::SUCC);
		return new Succ(name_);
	}
	case types::GETN: {
		Node* name_ = make_Name(token.value);
		eat(types::GETN);
		Node* num_ = num();
		eat(types::RPAREN);
		return new GetN(name_, num_);
	}
	case types::ADD: {
		eat(types::ADD);
		UnOp* unop_ = new UnOp(token, factor());
		return unop_;
	}
	case types::SUB: {
		eat(types::SUB);
		Node* unop = new UnOp(token, factor());
		return unop;
	}
	default:
		throw Parser_Error("Invalid syntax in a factor");
	}

	return new NoOp;
}

Node* Parser::pow() {
	Node* node = factor();

	while (current_token.type == types::POW) {
		Token token = current_token;
		eat(types::POW);
		node = new BinOp(token, node, factor());
		
	}

	return node;
}

Node* Parser::term() {
	Node* node = pow();

	while (current_token.type == types::MUL || current_token.type == types::DIV) {
		Token token = current_token;

		if (token.type == types::MUL)
			eat(types::MUL);
		if (token.type == types::DIV)
			eat(types::DIV);

		node = new BinOp(token,node, pow());
	}

	return node;
}

Node* Parser::expr() {
	Node* node = term();

	while (current_token.type == types::ADD || current_token.type == types::SUB) {
		Token token = current_token;

		if (token.type == types::ADD)
			eat(types::ADD);
		if (token.type == types::SUB)
			eat(types::SUB);

		node = new BinOp(token, node, term());
	}

	return node;
}


