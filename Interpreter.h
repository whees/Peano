#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "Node.h"
#include "Parser.h"
#include <stack>
#include <cmath>
#define SET_ERROR {throw Interpreter_Error("not a set.");}
using namespace std;

struct Interpreter_Error {
	string what;
	Interpreter_Error(string what_) : what{ what_ } {}
	Interpreter_Error() : what{ "Interpreter error!" } {}
};
struct Cpp_File {
	string header = "#include <iostream>\n#include \"Set.h\"\n";
	string declarations;
	string definitions;
	string main = "int main() {\n";
	string instantiations;
	string operations;
	string footer = "return 0;\n}";

	string get() {
		return header + declarations + definitions + main + instantiations + operations + footer;
	}
};
struct Serializer : Visitor {
	string value;
	static string get_value(Node* node_) {
		Serializer ser;
		node_->accept(&ser);
		return ser.value;
	}
	void return_(string value_) {
		value = value_;
	}
	void visit(GetN* getn_) override {
		string num = get_value(getn_->num);
		return_(get_value(getn_->name) + ".get(" + num + ")");
	}
	void visit(Num* num_) override {
		return_(num_->token.value);
	}
	void visit(Name* name_) override {
		return_(name_->token.value);
	}
	void visit(Program* prog) override {}
	void visit(Define* def_) override {}
	void visit(Show* show) override {}
	void visit(Get* get_) override {
		switch (get_->name->token.type) {
		case types::SELF:
			return_("__self__");
			return;
		case types::NAME:
			return_(get_value(get_->name) + ".get(__no__)");
			return;
		default:
			throw Interpreter_Error("cannot get value.");
		}
	}
	void visit(NoOp* noop) override {}
	void visit(Succ* succ_) override {

		return_(get_value(succ_->name) + ".get(__no__ + 1)");

	}
	void visit(Self* self_) override {
		return_("__self__");
	}
	void visit(Set* set) override {}
	void visit(BinOp* binop_) override {
		switch (binop_->token.type) {
		case types::ADD:
			return_(get_value(binop_->left) + "+" + get_value(binop_->right));
			return;
		case types::SUB:
			return_(get_value(binop_->left) + "-" + get_value(binop_->right));
			return;
		case types::MUL:
			return_(get_value(binop_->left) + "*" + get_value(binop_->right));
			return;
		case types::DIV:
			return_(get_value(binop_->left) + "/" + get_value(binop_->right));
			return;
		case types::POW:
			return_("pow(" + get_value(binop_->left) + "," + get_value(binop_->right) + ")");
			return;
		default:
			throw Interpreter_Error("cannot serialize node.");
		}
	}
	void visit(UnOp* unop_) override {
		switch (unop_->token.type) {
		case types::ADD:
			return_(get_value(unop_->arg));
			return;
		case types::SUB:
			return_("-" + get_value(unop_->arg));
			return;
		default:
			throw Interpreter_Error("cannot serialize node.");
		}
	}
	void visit(Type* type_) override {
		string type = type_->token.value;
		if (type == "int") {
			return_("int");
			return;
		}
		if (type == "uint") {
			return_("size_t");
			return;
		}
		if (type == "float") {
			return_("float");
			return;
		}
		throw Interpreter_Error("Unrecognized type.");
	}
};
struct Setter : Visitor {
	string base;
	string succ;

	static string get_base(Node* node_) {
		Setter setr;
		node_->accept(&setr);
		return setr.base;
	}
	static string get_succ(Node* node_) {
		Setter setr;
		node_->accept(&setr);
		return setr.succ;
	}

	void visit(GetN* getn_) override SET_ERROR
	void visit(Num* num_) override SET_ERROR
	void visit(Name* name_) override SET_ERROR
	void visit(Program* prog) override SET_ERROR
	void visit(Define* def_) override SET_ERROR
	void visit(Show* show) override SET_ERROR
	void visit(Get* get) override SET_ERROR
	void visit(NoOp* noop) override SET_ERROR
	void visit(Succ* succ) override SET_ERROR
	void visit(Self* self_) override SET_ERROR
	void visit(Set* set_) override {
		base = Serializer::get_value(set_->base);
		succ = Serializer::get_value(set_->succ);
	}
	void visit(BinOp* binop_) override SET_ERROR
	void visit(UnOp* unop_) override SET_ERROR
	void visit(Type* type_) override SET_ERROR
};
struct Interpreter : Visitor {
	Parser parser;
	Interpreter() : parser{ Parser() } {}
	Cpp_File file;

	void load(Parser parser_) {
		parser = parser_;
		try {
			parser.tree->accept(this);
		}
		catch (Parser_Error e) {
			throw Interpreter_Error(e.what);
		}
	}
	void visit(Program* prog_) override  {
		for (Node* arg_ : prog_->args)
			arg_->accept(this);
	}
	void visit(Define* def_) override {
		string type_ = Serializer::get_value(def_->type);
		string name_ = Serializer::get_value(def_->name);
		string succ_name_ = name_ + "_succ";
		string base_ = Setter::get_base(def_->set);
		string succ_ = Setter::get_succ(def_->set);

		file.declarations += "Set<" + type_ + "> " + name_ + ";\n";
		file.definitions += type_ + " " + succ_name_ + "(" + type_ + " __self__, size_t __no__) {\n"
			+ "return " +  succ_ + ";\n}\n";
		file.instantiations += name_ +  " = Set<" + type_ + ">" + "((" + type_ + ")" + base_ + ", " + succ_name_ + ")" + "; \n";
	}
	void visit(Show* show_) override  {
		string arg_ = Serializer::get_value(show_->arg);
		file.operations += "std::cout << " + arg_ + " << std::endl;\n";
	}
	void visit(GetN* getn_) override {}
	void visit(Num* num_) override {}
	void visit(Get* get) override {}
	void visit(Name* name) override {}
	void visit(NoOp* noop) override {}
	void visit(Succ* succ) override {}
	void visit(Self* self) override {}
	void visit(Set* set) override {}
	void visit(BinOp* binop) override {}
	void visit(UnOp* unop) override {}
	void visit(Type* type) override {}
};
#endif
