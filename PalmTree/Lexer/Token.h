#pragma once

#include <string>

enum class TokenType { 
	Keyword, Identifier, Operator, Number, String, Delimiter, EndOfFile
};

struct Token {
	TokenType type;
	std::string value;
	int position;

	std::string tokenTypeToString(const TokenType type_m) const {
		switch (type_m) {
		case TokenType::Keyword:
			return "Keyword";
		case TokenType::Identifier:
			return "Identifier";
		case TokenType::Operator:
			return "Operator";
		case TokenType::Number:
			return "Number";
		case TokenType::String:
			return "String";
		case TokenType::Delimiter:
			return "Delimiter";
		default:
			return "End Of File";
		}
	}

	std::string toString() const {
		return "{type: \"" + tokenTypeToString(type) + "\", value: \"" + value + "\"},";
	}
};