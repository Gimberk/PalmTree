#pragma once

#include <string>

enum class TokenType { 
	Keyword, Identifier, Operator, Number, String, Delimiter, EndOfFile 
};

struct Token {
	TokenType type;
	std::string value;
	int position;
};