#include "Lexer.h"

void Lexer::load(File file_) {
	if (file_.buffer.size() == 0)
		throw Lexer_Error("Empty file.");
	buffer = file_.buffer;
	pos = 0;
	current_char = buffer[pos];
}

void Lexer::advance() {
	pos++;
	if (pos >= buffer.size())
		current_char = END;
	else
		current_char = buffer[pos];
}

char Lexer::peek() {
	int peek_pos = pos + 1;
	if (peek_pos >= buffer.size())
		return END;
	return buffer[peek_pos];
}

Token Lexer::name() {
	string result;

	while (isalnum(current_char)) {
		result += current_char;
		advance();
	}

	if (RESERVED_KEYWORDS.count(result))
		return RESERVED_KEYWORDS[result];

	if (current_char == '(') {
		advance();
		if (current_char == ')') {
			advance();
			return Token(types::GET, result);
		}
		if (current_char == '+' && peek() == ')') {
			advance();
			advance();
			return Token(types::SUCC, result);
		}
		if (isdigit(current_char))
			return Token(types::GETN, result);

		throw Lexer_Error("Invalid glyph.");
	}

	return Token(types::NAME, result);
}

Token Lexer::num() {
	string result = "";

	while (isdigit(current_char)) {
		result += current_char;
		advance();
	}

	return Token(types::NUM, result);
}

void Lexer::skip_space() {
	while (current_char == ' ')
		advance();
}

Token Lexer::get_next_token() {
	while (current_char != END) {
		if (current_char == ' ') {
			skip_space();
			continue;
		}
		if (current_char == NL) {
			advance();
			continue;
		}
		
		if (isalpha(current_char))
			return name();

		if (isdigit(current_char))
			return num();

		if (current_char == '=') {
			advance();
			return Token(types::ASSIGN, "=");
		}

		if (current_char == ';') {
			advance();
			return Token(types::SEMI, ";");
		}
		
		if (current_char == '+') {
			advance();
			return Token(types::ADD, "+");
		}

		if (current_char == '-') {
			advance();
			return Token(types::SUB, "-");
		}

		if (current_char == '*') {
			advance();
			return Token(types::MUL, "*");
		}

		if (current_char == '/') {
			advance();
			return Token(types::DIV, "/");
		}

		if (current_char == '{') {
			advance();
			return Token(types::LBRACK, "{");
		}

		if (current_char == '}') {
			advance();
			return Token(types::RBRACK, "}");
		}

		if (current_char == '(') {
			advance();

			if (current_char == ')') {
				advance();
				return Token(types::GET, "()");
			}
			if (current_char == '+' && peek() == ')') {
				advance();
				advance();
				return Token(types::SUCC, "(+)");
			}

			return Token(types::LPAREN, "(");
		}
		
		if (current_char == ')') {
			advance();
			return Token(types::RPAREN, ")");
		}

		if (current_char == '|') {
			advance();
			return Token(types::PIPE, "|");
		}

		if (current_char == ':') {
			advance();
			return Token(types::DEF, ":");
		}

		if (current_char == '^') {
			advance();
			return Token(types::POW, "^");
		}
		
		throw Lexer_Error("Invalid glyph.");
	}

	return Token(types::EoF, "EoF");
}











