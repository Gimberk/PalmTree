#include "Lexer.h"
#include <iostream>

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
	if (identifier == "let") return { TokenType::Keyword, "let" };
	else if (identifier == "print") return { TokenType::Keyword, "print" };

	// if it doesn't match a key word, it's an identifier.
	return { TokenType::Identifier, identifier };
}