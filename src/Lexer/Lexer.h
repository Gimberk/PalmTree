#pragma once

#include "../Types/Token.h"
#include "../Parser/AST.h"

#include <functional>
#include <vector>

class Lexer
{
public:
	const static std::unordered_map<std::string, TokenType> KEYWORDS;
	const static std::unordered_map<std::string,
		std::function<Value(const std::vector<Value>&)>> BUILT_IN_FUNCTIONS;
public:
	static std::vector<Token> tokenize(const std::string& code);
private:
	static Token readNumber(size_t& pos, const std::string& code);
	static Token readIdentifierKeyword(size_t& pos, const std::string& code);
};