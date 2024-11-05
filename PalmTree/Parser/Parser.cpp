#include "Parser.h"
#include "../Util/Util.h"

#include <stdexcept>

void Parser::advance() {
    if (currentPosition < tokens.size()) {
        currentPosition++;
    }
}

void Parser::expect(const TokenType type, const std::string& value) {
    Token currentToken = peek();

    if (currentToken.type != type || currentToken.value != value) {
        throw std::runtime_error("Expected token " + Util::tokenTypeToString(type) +
            " with value '" + value + "' but found '" + currentToken.value + "'");
    }
    advance(); // Move to the next token if it matches
}

Token Parser::expect(const TokenType type) {
    if (peek().type != type) {
        throw std::runtime_error("Expected token of type " + Util::tokenTypeToString(type) +
            " but found '" + peek().value + "'");
    }
    advance(); // Move to the next token if it matches
}

Token Parser::peek() {
    // Check if we're at the end of the tokens list
    if (currentPosition < tokens.size()) {
        return tokens[currentPosition];
    }
    // Return a special EndOfFile token if no more tokens are available
    return Token{ TokenType::EndOfFile, "" };
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    Token current = peek();

    // Check the type of the current token
    if (current.type == TokenType::Number) {
        advance();
        return std::make_unique<NumberNode>(current.value); // Create a node for a number
    }
    else if (current.type == TokenType::Identifier) {
        advance();
        return std::make_unique<VariableNode>(current.value); // Create a node for an identifier (variable)
    }
    else if (current.type == TokenType::Delimiter && current.value == "(") {
        // Handle grouped expressions (e.g., expressions in parentheses)
        advance(); // Consume '('
        std::unique_ptr<ASTNode> expr = parseExpression();
        expect(TokenType::Delimiter, ")"); // Expect and consume ')'
        return expr;
    }

    throw std::runtime_error("Unexpected token in parseTerm");
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    std::unique_ptr<ASTNode> left = parseTerm();

    while (peek().type == TokenType::Operator && (peek().value == "+" || peek().value == "-")) {
        Token op = peek(); // Look at the operator but don't consume it yet
        advance();         // Now consume the operator
        std::unique_ptr<ASTNode> right = parseTerm();
        left = std::make_unique<BinaryOperationNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseStatement()
{
    if (peek().type == TokenType::Keyword && peek().value == "let") 
        return parseVariableDeclaration();
    else if (peek().type == TokenType::Identifier)
        return parseFunctionCall();
    throw std::runtime_error("Unexpected token in statement");
}

std::unique_ptr<ASTNode> Parser::parseVariableDeclaration() {
    expect(TokenType::Keyword, "let");
    Token identifier = expect(TokenType::Identifier);
    expect(TokenType::Operator, "=");
    std::unique_ptr<ASTNode> value = parseExpression();
    expect(TokenType::Delimiter, ";");

    return std::make_unique<VariableDeclarationNode>(identifier.value, std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseFunctionCall()
{
    Token functionToken = expect(TokenType::Identifier);
    expect(TokenType::Delimiter, "(");

    std::vector<std::unique_ptr<ASTNode>> args;
    if (peek().type != TokenType::Delimiter || peek().value != ")") {
        // Parse arguments if any
        args.push_back(parseExpression());
        while (peek().type == TokenType::Delimiter && peek().value == ",") {
            advance(); // Skip comma
            args.push_back(parseExpression());
        }
    }

    expect(TokenType::Delimiter, ")");
    expect(TokenType::Delimiter, ";"); // Enforce semicolon after function call

    return std::make_unique<FunctionCallNode>(functionToken.value, std::move(args));
}