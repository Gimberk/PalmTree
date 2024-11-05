#pragma once

#include <vector>
#include "../PalmTree.h"

class Lexer
{
public:
	static std::vector<Token> tokenize(const std::string& code);
private:
	static Token readNumber(size_t& pos, const std::string& code);
	static Token readIdentifierKeyword(size_t& pos, const std::string& code);
};