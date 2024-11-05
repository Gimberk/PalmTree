#pragma once

#include <vector>
#include "PalmTree.h"

class Lexer
{
public:
	std::vector<Token> tokenize(const std::string& code);
private:
	Token readNumber(size_t& pos, const std::string& code);
	Token readIdentifierKeyword(size_t& pos, const std::string& code);
};