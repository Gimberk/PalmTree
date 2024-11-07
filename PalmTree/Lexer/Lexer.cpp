#include "Lexer.h"
#include <iostream>

const std::unordered_map<std::string, TokenType> Lexer::KEYWORDS = {
	{ "let", TokenType::LetKeyword }
};

/*
temporary way of defining all built-in functions 
(it's not great for more complex functions, but it works for print) ;-) :|
*/
const std::unordered_map<std::string, 
	std::function<void(const std::vector<int>&)>> Lexer::BUILT_IN_FUNCTIONS = {
	{	"print", [](const std::vector<int>& args) {
			for (int arg : args) std::cout << arg << " ";
			std::cout << std::endl;
		}
	}
};


std::vector<Token> Lexer::tokenize(const std::string& code)
{
	std::vector<Token> tokens;
	size_t pos = 0;

	while (pos < code.length()) {
		const char curr = code[pos];
		if (std::isdigit(curr)) tokens.push_back(readNumber(pos, code));
		else if (std::isalpha(curr)) tokens.push_back(readIdentifierKeyword(pos, code));
		else if (curr == '%') {
			tokens.push_back({ TokenType::Operator, "%", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == '+') {
			tokens.push_back({ TokenType::Operator, "+", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == '-') {
			tokens.push_back({ TokenType::Operator, "-", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == '/') {
			tokens.push_back({ TokenType::Operator, "/", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == '*') {
			tokens.push_back({ TokenType::Operator, "*", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == '=') {
			tokens.push_back({ TokenType::Operator, "=", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == ';') {
			tokens.push_back({ TokenType::Delimiter, ";", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == '(') {
			tokens.push_back({ TokenType::Delimiter, "(", static_cast<int>(pos) });
			pos++;
		}
		else if (curr == ')') {
			tokens.push_back({ TokenType::Delimiter, ")", static_cast<int>(pos) });
			pos++;
		}
		else pos++;
	}

	tokens.push_back({ TokenType::EndOfFile, "EOF" });
	return tokens;
}

Token Lexer::readNumber(size_t& pos, const std::string& code)
{
	std::string number;
	while (pos < code.length() && ((std::isdigit(code[pos]) || code[pos] == '.'))) number += code[pos++];
	return { TokenType::Number, number, static_cast<int>(pos) };
}

Token Lexer::readIdentifierKeyword(size_t& pos, const std::string& code)
{
	std::string identifier;
	while (pos < code.length() && std::isalnum(code[pos])) identifier += code[pos++];

	// for now, list all keywords here
	if (Lexer::KEYWORDS.find(identifier) != Lexer::KEYWORDS.end())
		return Token{ Lexer::KEYWORDS.at(identifier), identifier };

	// if it doesn't match a key word, it's an identifier.
	return { TokenType::Identifier, identifier };
}