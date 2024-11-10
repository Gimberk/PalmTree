#include "Lexer.h"
#include <iostream>
#include <iomanip>

const std::unordered_map<std::string, TokenType> Lexer::KEYWORDS = {
	{ "let", TokenType::LetKeyword }
};

/*
temporary way of defining all built-in functions 
(it's not great for more complex functions, but it works for print) ;-) :|
*/
const std::unordered_map<std::string, 
	std::function<void(const std::vector<Value>&)>> Lexer::BUILT_IN_FUNCTIONS = {
	{	"print", [](const std::vector<Value>& args) {
			for (const auto& val : args) {
				if (val.isDouble()) {
					const int count = val.decimalCount();
					std::cout << std::fixed << std::setprecision(count) << val.asDouble() << " ";
				}
				else if (val.isInt()) std::cout << val.asInt() << " ";
			}
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
	std::string numberStr;

	while (isdigit(code[pos]) || code[pos] == '.') numberStr += code[pos++];

	if (numberStr.find('.') != std::string::npos) return { TokenType::Double, numberStr };
	else return { TokenType::Int, numberStr };
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