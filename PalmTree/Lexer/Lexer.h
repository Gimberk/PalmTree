#pragma once

#include "../PalmTree.h"

#include <functional>
#include <vector>

class Lexer
{
public:
	const static std::unordered_map<std::string, TokenType> KEYWORDS;
	const static std::unordered_map<std::string,
		std::function<void(const std::vector<int>&)>> BUILT_IN_FUNCTIONS;
public:
	static std::vector<Token> tokenize(const std::string& code);
private:
	static Token readNumber(size_t& pos, const std::string& code);
	static Token readIdentifierKeyword(size_t& pos, const std::string& code);
};