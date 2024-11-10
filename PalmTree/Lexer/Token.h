#pragma once

#include <string>
#include <unordered_map>

enum class TokenType { 
	Keyword, Identifier, Operator, Delimiter, EndOfFile,

	Int, Double, String,

	LetKeyword
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
		case TokenType::Delimiter:
			return "Delimiter";
		case TokenType::LetKeyword:
			return "Keyword";
		case TokenType::Int:
			return "Integer";
		case TokenType::Double:
			return "Double";
		case TokenType::String:
			return "String";
		default:
			return "End Of File";
		}
	}

	std::string to_string() const {
		return "{type: \"" + tokenTypeToString(type) + "\", value: \"" + value + "\"},";
	}
};