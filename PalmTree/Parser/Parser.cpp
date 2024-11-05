#include "Parser.h"

#include <stdexcept>

bool Parser::expect(const TokenType type, const std::string value) {
    return (tokens[current].type == type && tokens[current].value == value);
}

bool Parser::expect(const TokenType type) {
    return tokens[current].type == type;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> nodes;

    while (current < tokens.size() && tokens[current].type != TokenType::EndOfFile) {
        if (tokens[current].type == TokenType::Keyword && tokens[current].value == "let") {
            nodes.push_back(parseVariableDeclaration());
        }
        else if (tokens[current].type == TokenType::Keyword && tokens[current].value == "print") {
            nodes.push_back(parsePrintStatement());
        }
        else {
            throw std::runtime_error("Unexpected token");
        }
    }

    return nodes;
}

std::unique_ptr<ASTNode> Parser::parseVariableDeclaration() {
    current++;  // skip "let"

    if (tokens[current].type != TokenType::Identifier) {
        throw std::runtime_error("Expected variable name after 'let'");
    }

    std::string varName = tokens[current].value;
    current++;  // move to '='

    if (tokens[current].type != TokenType::Operator || tokens[current].value != "=") {
        throw std::runtime_error("Expected '=' in variable declaration");
    }

    current++;  // move to the value

    if (tokens[current].type != TokenType::Number) {
        throw std::runtime_error("Expected a number after '='");
    }

    int value = std::stoi(tokens[current].value);
    current++;  // move past the value

    if (tokens[current].type != TokenType::Delimiter || tokens[current].value != ";") {
        throw std::runtime_error("Expected ';' at the end of variable declaration");
    }

    current++;  // move past ';'

    return std::make_unique<VariableDeclarationNode>(varName, value);
}

std::unique_ptr<ASTNode> Parser::parsePrintStatement() {
    current++;  // skip "print"

    if (tokens[current].type != TokenType::Delimiter || tokens[current].value != "(") {
        throw std::runtime_error("Expected '(' after 'print'");
    }

    current++;  // move to the variable name

    if (tokens[current].type != TokenType::Identifier) {
        throw std::runtime_error("Expected variable name in print statement");
    }

    std::string varName = tokens[current].value;
    current++;  // move to ')'

    if (tokens[current].type != TokenType::Delimiter || tokens[current].value != ")") {
        throw std::runtime_error("Expected ')' after variable name");
    }

    current++;  // move to ';'

    if (tokens[current].type != TokenType::Delimiter || tokens[current].value != ";") {
        throw std::runtime_error("Expected ';' at the end of print statement");
    }

    current++;  // move past ';'

    return std::make_unique<PrintNode>(varName);
}