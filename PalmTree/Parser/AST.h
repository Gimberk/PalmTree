#pragma once

#include <string>

struct ASTNode {
    virtual std::string to_string(int indent = 0) const = 0;  // for debug purposes to visualize the AST
    virtual ~ASTNode() = default;
};

struct VariableDeclarationNode : public ASTNode {
    std::string name;
    int value;

    VariableDeclarationNode(const std::string& name, int val)
        : name(name), value(val) {}

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "VariableDeclarationNode: " + name + " = " + std::to_string(value);
    }
};

struct PrintNode : public ASTNode {
    std::string name;

    PrintNode(const std::string& name)
        : name(name) {}

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "PrintNode: " + name;
    }
};