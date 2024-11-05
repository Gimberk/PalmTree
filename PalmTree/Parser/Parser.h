#pragma once

#include "AST.h"

class Parser
{
public:
	Parser(std::vector<Token> tokens) : currentPosition(0), tokens(tokens) {}

	std::unique_ptr<ASTNode> parseStatement();
	std::unique_ptr<ASTNode> parseFunctionCall();
	std::unique_ptr<ASTNode> parseExpression();
	std::unique_ptr<ASTNode> parseTerm();
	std::unique_ptr<ASTNode> parseVariableDeclaration();
private:
	size_t currentPosition;
	std::vector<Token> tokens;
private:
	void advance();
	void expect(const TokenType type, const std::string& code);
	Token expect(const TokenType type);
	Token peek();
};