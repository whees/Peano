#ifndef NODE_H
#define NODE_H
#include "Token.h"
#include <iostream>
#include <vector>
#define VISITABLE void accept(Visitor* visitor) override { return visitor->visit(this); };
using namespace std;

struct Get;
struct GetN;
struct Set;
struct Num;
struct Name;
struct NoOp;
struct Program;
struct Define;
struct Show;
struct Succ;
struct BinOp;
struct UnOp;
struct Self;
struct Type;

struct Visitor {
	virtual void visit(Get* get) = 0;
	virtual void visit(Set* set) = 0;
	virtual void visit(Num* num) = 0;
	virtual void visit(Name* name) = 0;
	virtual void visit(NoOp* noop) = 0;
	virtual void visit(Program* prog) = 0;
	virtual void visit(Define* def) = 0;
	virtual void visit(Show* show) = 0;
	virtual void visit(Succ* succ) = 0;
	virtual void visit(BinOp* binop) = 0;
	virtual void visit(UnOp* unop) = 0;
	virtual void visit(GetN* getn) = 0;
	virtual void visit(Self* self) = 0;
	virtual void visit(Type* type) = 0;
};

struct Node {
	Token token;
	virtual void accept(Visitor* visitor) = 0;
};

struct NoOp : Node {
	NoOp() { token = Token(types::NONE, "none"); }
	VISITABLE
};

struct Program : Node {
	vector<Node*> args;
	Program() { token = Token(types::PROGRAM, "program"); }
	void append(Node* node) { args.push_back(node); }
	VISITABLE
};

struct Num : Node {
	Num(Token token_) { token = token_; }
	VISITABLE
};

struct Name : Node {
	Name(Token token_) { token = token_; }
	VISITABLE
};

struct Set : Node {
	Node* base;
	Node* succ;
	Set(Node* base_, Node* succ_) : base{ base_ }, succ{ succ_ } { token = Token(types::SET, "set"); }
	VISITABLE
};

struct BinOp : Node {
	Node* left;
	Node* right;
	BinOp(Token token_, Node* left_, Node* right_) : left{ left_ }, right{ right_ } { token = token_; }
	VISITABLE
};

struct UnOp : Node {
	Node* arg;
	UnOp(Token token_, Node* arg_) : arg{ arg_ } { token = token_; }
	VISITABLE
};

struct Define : Node {
	Node* name;
	Node* set;
	Node* type;
	Define(Node* type_, Node* name_, Node* set_) : type{ type_ }, name { name_ }, set{ set_ } { token = Token(types::DEF, "def"); }
	VISITABLE
};

struct Show : Node {
	Node* arg;
	Show(Node* arg_) : arg{ arg_ } { token = Token(types::SHOW, "show"); }
	VISITABLE
};

struct Get : Node {
	Node* name;
	Get(Node* name_) : name{ name_ } { token = Token(types::GET, "__get__"); }
	VISITABLE
};

struct GetN : Node {
	Node* name;
	Node* num;
	GetN(Node* name_, Node* num_) : name{ name_ }, num{ num_ } {token = Token(types::GETN, "getn"); }

	VISITABLE
};

struct Succ : Node {
	Node* name;
	Succ(Node* name_) : name{ name_ } { token = Token(types::SUCC, "succ"); }
	VISITABLE
};

struct Self : Node {
	Self() {token = Token(types::SELF, "__self__"); }
	VISITABLE
};

struct Type : Node {
	Type(Token token_) { token = token_; }
	VISITABLE
};

Node* make_Num(string value);
Node* make_Name(string value);
#endif





