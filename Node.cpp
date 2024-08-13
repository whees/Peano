#include "Node.h"

Node* make_Num(string value) {
	return new Num(Token(types::NUM, value));
}

Node* make_Name(string value) {
	return new Name(Token(types::NAME, value));
}







