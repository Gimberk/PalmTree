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
	bool check(TokenType type) const;
	bool checkNext(TokenType type) const;

	bool check(TokenType type, std::string value) const;
	bool checkNext(TokenType type, std::string value) const;
	bool match(TokenType type, std::string value);

	void advance();
	const Token& expect(TokenType type);
	const Token& expect(TokenType type, const std::string& value);

	Token previous();

	std::unique_ptr<ExpressionNode> parseExpression();
	std::unique_ptr<ExpressionNode> parseTerm();
	std::unique_ptr<ExpressionNode> parseAdditionSubtraction();
	std::unique_ptr<ExpressionNode> parseMultiplicationDivision();
	std::unique_ptr<ExpressionNode> parsePrimary();
private:
	std::unique_ptr<ASTNode> parseFunctionOrExpression();
	std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration();
	std::unique_ptr<ASTNode> parseFunctionCall();
};