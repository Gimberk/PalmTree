#pragma once

#include "../PalmTree.h"

#include <vector>
#include <memory>

struct ASTNode {
	virtual ~ASTNode() = default;
};

struct FunctionCallNode : public ASTNode {
	std::string name;
	std::vector<std::unique_ptr<ASTNode>> arguments;
};

struct BinaryOperationNode : public ASTNode {
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOperationNode(const Token& operatorToken, 
                        std::unique_ptr<ASTNode> leftNode, 
                        std::unique_ptr<ASTNode> rightNode)
        : op(operatorToken.value), left(std::move(leftNode)), right(std::move(rightNode)) {}
};

struct VariableDeclarationNode : public ASTNode {
    std::string name;
    std::unique_ptr<ASTNode> value;

    VariableDeclarationNode(const std::string& name, std::unique_ptr<ASTNode> val)
        : name(name), value(std::move(val)) {}
};

struct VariableNode : public ASTNode {
    std::string name;

    VariableNode(const std::string& name)
        : name(name) {}
};

struct NumberNode : public ASTNode {
    double value;

    NumberNode(const std::string& numStr)
        : value(std::stod(numStr)) {}
};