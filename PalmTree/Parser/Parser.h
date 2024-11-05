#pragma once

#include "AST.h"
#include "../Lexer/Token.h"

#include <memory>
#include <vector>

class Parser
{
public:
	Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

	std::vector<std::unique_ptr<ASTNode>> parse();
private:
	std::vector<Token> tokens;
	size_t current;
private:
	bool expect(const TokenType type, const std::string value);
	bool expect(const TokenType type);
private:
	std::unique_ptr<ASTNode> parseVariableDeclaration(); 
private:
	std::unique_ptr<ASTNode> parsePrintStatement();
};