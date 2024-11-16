#include "Parser.h"

#include <stdexcept>

//
// HELPER METHODS
//

bool Parser::check(TokenType type, std::string value) const {
    return !isAtEnd() && tokens[current].type == type && tokens[current].value == value;
}

bool Parser::check(TokenType type) const {
    return !isAtEnd() && tokens[current].type == type;
}

bool Parser::checkNext(TokenType type) const {
    return !isAtEnd() && current + 1 < tokens.size() && tokens[current + 1].type == type;
}

bool Parser::checkNext(TokenType type, std::string value) const {
    return !isAtEnd()
        && current + 1 < tokens.size()
        && tokens[current + 1].type == type
        && tokens[current + 1].value == value;
}

const Token& Parser::expect(TokenType type) {
    if (isAtEnd() || tokens[current].type != type) {
        std::cout << (tokens[current].type != type);
        std::cout << "\nExpected " + tokens[current].tokenTypeToString(type) +
            " but got " + tokens[current].tokenTypeToString(tokens[current].type) + "\n";
        throw std::runtime_error("Expected " + tokens[current].tokenTypeToString(type) +
            " but got " + tokens[current].tokenTypeToString(type));
    }
    return tokens[current++];
}

const Token& Parser::expect(TokenType type, const std::string& value) {
    if (isAtEnd() || tokens[current].type != type || tokens[current].value != value) {
        std::cout << "\nExpected " + tokens[current].tokenTypeToString(type) +
            " but got " + tokens[current].tokenTypeToString(tokens[current].type) + "\n";
        throw std::runtime_error("Expected " + tokens[current].tokenTypeToString(type) +
            " but got " + tokens[current].tokenTypeToString(type));
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
        if (match(TokenType::LetKeyword))
            statements.push_back(parseVariableDeclaration());
        else if (check(TokenType::Identifier) && checkNext(TokenType::Operator, "="))
            statements.push_back(parseAssignment());
        else if (match(TokenType::Identifier))
            statements.push_back(parseFunctionOrExpression());
        else throw std::runtime_error("Unexpected token");
    }
    return std::make_unique<ProgramNode>(std::move(statements));
}



std::unique_ptr<ASTNode> Parser::parseFunctionOrExpression() {
    if (check(TokenType::Delimiter, "(")) {
        std::unique_ptr<ExpressionNode> v = parseFunctionCall();
        expect(TokenType::Delimiter, ";");
        return v;
    }
    else return parseExpression();
}

std::unique_ptr<ExpressionNode> Parser::parseTerm() {
    if (match(TokenType::Int))
        return std::make_unique<NumberNode>(std::stoi(previous().value));
    else if (match(TokenType::Double))
        return std::make_unique<NumberNode>(std::stod(previous().value));
    else if (match(TokenType::Identifier))
        return std::make_unique<VariableNode>(previous().value);

    throw std::runtime_error("Expected a term (number or identifier)");
}

std::unique_ptr<ExpressionNode> Parser::parseExpression() {
    if (current < tokens.size() - 2 && check(TokenType::Delimiter, "(") && tokens[current + 2].value == ",") 
        return parseLambdaExpression();
    else if (checkNext(TokenType::Operator, "|>")) return parsePipeExpression();
    else return parseAdditionSubtraction();
}

std::unique_ptr<LambdaNode> Parser::parseLambdaExpression() {
    std::string funcName = tokens[current-2].value;
    expect(TokenType::Delimiter, "(");
    std::vector<std::string> arguments;

    if (!check(TokenType::Delimiter, ")")) {
        do {
            arguments.push_back(expect(TokenType::Identifier).value);
        } while (match(TokenType::Delimiter, ","));
    }
    expect(TokenType::Delimiter, ")");
    expect(TokenType::Operator, "=>");
    std::unique_ptr<ExpressionNode> body = parseExpression();

    return std::make_unique<LambdaNode>(funcName, arguments, std::move(body));
}

std::unique_ptr<ExpressionNode> Parser::parsePipeExpression() {
    std::unique_ptr<ExpressionNode> left = parsePrimary();
    while (match(TokenType::Operator, "|>")) {
        if (!check(TokenType::Identifier))
            throw std::runtime_error("Expected function name after '|>' operator");
        std::string functionName = expect(TokenType::Identifier).value;
        std::vector<std::unique_ptr<ExpressionNode>> arguments;
        arguments.push_back(std::move(left));

        if (match(TokenType::Delimiter, "(")) {
            do {
                arguments.push_back(parseExpression());
            } while (match(TokenType::Delimiter, ","));
            expect(TokenType::Delimiter, ")");
        }
        left = std::make_unique<FunctionCallNode>(functionName, std::move(arguments));
    }
    return left;
}

std::unique_ptr<ExpressionNode> Parser::parseAdditionSubtraction() {
    std::unique_ptr<ExpressionNode> left = parseMultiplicationDivision();

    while (match(TokenType::Operator)
        && (previous().value == "+"
            || previous().value == "-")) {
        char op = previous().value[0];
        std::unique_ptr<ExpressionNode> right = parseMultiplicationDivision();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));
    }

    return left;
}

std::unique_ptr<ExpressionNode> Parser::parseMultiplicationDivision() {
    std::unique_ptr<ExpressionNode> left = parsePrimary();

    bool isOp = match(TokenType::Operator),
        isMulOrDiv = previous().value == "*" || previous().value == "/";
    while (isOp && isMulOrDiv) {
        char op = previous().value[0];
        std::unique_ptr<ExpressionNode> right = parsePrimary();
        left = std::make_unique<BinaryOperationNode>(std::move(left), op, std::move(right));

        isOp = match(TokenType::Operator);
        isMulOrDiv = previous().value == "*" || previous().value == "/";
    }
    if (isOp && !isMulOrDiv) current--;
    return left;
}

std::unique_ptr<ExpressionNode> Parser::parsePrimary() {
    if (match(TokenType::Int))
        return std::make_unique<NumberNode>(std::stoi(previous().value));
    else if (match(TokenType::Double))
        return std::make_unique<NumberNode>(std::stod(previous().value));
    else if (match(TokenType::Identifier)) {
        std::string identifier = previous().value;
        if (check(TokenType::Delimiter, "(")) {
            return parseFunctionCall();
        }
        return std::make_unique<VariableNode>(identifier);
    }
    else if (match(TokenType::Delimiter) && previous().value == "(") {
        std::unique_ptr<ExpressionNode> expr = parseExpression();
        expect(TokenType::Delimiter, ")");
        return expr;
    }
    throw std::runtime_error("Unexpected token in expression");
}

std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration() {
    const std::string varName = expect(TokenType::Identifier).value;
    const bool mut = match(TokenType::MutableKeyword);

    std::optional<std::unique_ptr<ExpressionNode>> expr;
    std::optional<std::shared_ptr<LambdaNode>> lambdaExpr;
    if (match(TokenType::Operator, "=")) expr = parseExpression();
    if (tokens[current - 1].value != ";") expect(TokenType::Delimiter, ";");

    return std::make_unique<VariableDeclarationNode>(varName, std::move(expr), mut);
}

std::unique_ptr<ExpressionNode> Parser::parseFunctionCall() {
    std::string functionName = previous().value;
    expect(TokenType::Delimiter, "(");

    std::vector<std::unique_ptr<ExpressionNode>> arguments;

    if (!check(TokenType::Delimiter, ")")) {
        do {
            arguments.push_back(parseExpression());
        } while (match(TokenType::Delimiter, ","));
    }

    expect(TokenType::Delimiter, ")");
    return std::make_unique<FunctionCallNode>(functionName, std::move(arguments));
}

std::unique_ptr<AssignmentNode> Parser::parseAssignment() {
    std::string name = expect(TokenType::Identifier).value;
    expect(TokenType::Operator, "=");
    std::unique_ptr<ExpressionNode> expression = parseExpression();
    expect(TokenType::Delimiter, ";");
    return std::make_unique<AssignmentNode>(name, std::move(expression));
}