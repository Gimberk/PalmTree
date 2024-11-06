#include "Parser.h"

#include <stdexcept>

//
// HELPER METHODS
//

const Token& Parser::expect(TokenType type) {
    if (isAtEnd() /* || tokens[current].type != type*/) {
        std::cout << (tokens[current].type != type);
        std::cout << "\nExpected " + tokens[current].tokenTypeToString(type) + " but got " + tokens[current].tokenTypeToString(type) + "\n";
        throw std::runtime_error("Expected " + tokens[current].tokenTypeToString(type) + " but got " + tokens[current].tokenTypeToString(type));
    }
    return tokens[current++];
}

const Token& Parser::expect(TokenType type, const std::string& value) {
    if (isAtEnd() || tokens[current].type != type || tokens[current].value != value) {
        throw std::runtime_error("Unexpected token or value");
    }
    return tokens[current++];
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::EndOfFile;
}

bool Parser::match(TokenType type) {
    if (!isAtEnd() && tokens[current].type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(TokenType type, std::string value) {
    if (!isAtEnd() && tokens[current].type == type && tokens[current].value == value) {
        advance();
        return true;
    }
    return false;
}

void Parser::advance() {
    if (!isAtEnd()) current++;
}

Token Parser::previous() {
    if (current <= 0) throw std::runtime_error("Invalid operation");
    return tokens[current - 1];
}

//
// PARSER
//

std::unique_ptr<ProgramNode> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!isAtEnd()) {
        if (match(TokenType::Keyword, std::string("let"))) {
            statements.push_back(parseVariableDeclaration());
        }
        else if (match(TokenType::Keyword, std::string("print"))) {
            statements.push_back(parsePrintStatement());
        }
        else {
            throw std::runtime_error("Unexpected statement type.");
        }
    }

    return std::make_unique<ProgramNode>(std::move(statements));
}

std::unique_ptr<ExpressionNode> Parser::parseTerm() {
    if (match(TokenType::Number)) return std::make_unique<NumberNode>(std::stoi(previous().value));
    else if (match(TokenType::Identifier)) return std::make_unique<VariableNode>(previous().value);

    throw std::runtime_error("Expected a term (number or identifier)");
}

std::unique_ptr<ExpressionNode> Parser::parseExpression() {
    std::unique_ptr<ExpressionNode> left = parseTerm();

    while (match(TokenType::Operator)) {
        char op = previous().value[0];  // Assumes single character operators like '+', '-', etc.
        std::unique_ptr<ExpressionNode> right = parseTerm();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration() {
    //expect(TokenType::Keyword);  // 'let' keyword
    std::string varName = expect(TokenType::Identifier).value;
    expect(TokenType::Operator);  // '=' operator

    // Parse the initializer expression
    std::unique_ptr<ExpressionNode> initializer = parseExpression();

    expect(TokenType::Delimiter, ";");  // ';' at the end of the declaration
    return std::make_unique<VariableDeclarationNode>(varName, std::move(initializer));
}

std::unique_ptr<PrintNode> Parser::parsePrintStatement() {
    //expect(TokenType::Keyword);  // 'print' keyword
    expect(TokenType::Delimiter, "(");  // '(' symbol

    // Parse the argument expression for print
    std::unique_ptr<ExpressionNode> argument = parseExpression();

    expect(TokenType::Delimiter, ")");  // ')' symbol
    expect(TokenType::Delimiter, ";");  // ';' symbol at the end of the statement

    return std::make_unique<PrintNode>(std::move(argument));
}