#pragma once

#include "AST.h"
#include "../Lexer/Token.h"

#include <memory>
#include <vector>

class Parser
{
public:
	Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

	std::unique_ptr<ProgramNode> parse();
private:
	std::vector<Token> tokens;
	size_t current;
private:
	bool isAtEnd() const;
	bool match(TokenType type);
	bool match(TokenType type, std::string value);

	void advance();
	const Token& expect(TokenType type);
	const Token& expect(TokenType type, const std::string& value);

	Token previous();

	std::unique_ptr<ExpressionNode> parseExpression();
	std::unique_ptr<ExpressionNode> parseTerm();
private:
	std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration();
private:
	std::unique_ptr<PrintNode> parsePrintStatement();
};