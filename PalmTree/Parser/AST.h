#pragma once

#include "../Value.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <optional>

struct ASTNode {
    virtual std::string to_string(int indent = 0) const = 0;  // for debug purposes to visualize the AST
    virtual void visit(std::unordered_map<std::string, Value>& variables,
                        const std::unordered_map<std::string,
                        std::function<void(const std::vector<Value>&)>>&builtInFunctions) const = 0;
    virtual ~ASTNode() = default;
};

struct ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    ProgramNode(std::vector<std::unique_ptr<ASTNode>> stmts) : statements(std::move(stmts)) {}

    void visit(std::unordered_map<std::string, Value>& variables,
                const std::unordered_map<std::string,
                std::function<void(const std::vector<Value>&)>>&builtInFunctions) const override {
        for (const auto& stmt : statements) stmt->visit(variables, builtInFunctions);
    }

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "PROGRAM NODE";
    }
};

// parent class for expressions of all types
struct ExpressionNode : public ASTNode {
    virtual Value evaluate(const std::unordered_map<std::string, Value>& variables) const = 0;
};

// number literals like 5
struct NumberNode : public ExpressionNode {
    Value value;

    NumberNode(Value val) : value(val) {}

    Value evaluate(const std::unordered_map<std::string, Value>& variables) const override {
        return value;
    }

    void visit(std::unordered_map<std::string, Value>& variables,
                const std::unordered_map<std::string,
                std::function<void(const std::vector<Value>&)>>&builtInFunctions) const override {
        /* Doesn't do anything right now */
    }

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "NUMBER-LIT (" + value.to_string(); +")";
    }
};

// for identifiers like print(x); (x is considered a variable-node)
struct VariableNode : public ExpressionNode {
    std::string name;
    VariableNode(const std::string& name) : name(name) {}

    Value evaluate(const std::unordered_map<std::string, Value>& variables) const override {
        auto it = variables.find(name);
        if (it != variables.end()) return it->second;
        else {
            std::cout << "Undefined variable: " + name << "\n";
            throw std::runtime_error("Undefined variable: " + name);
        }
    }

    void visit(std::unordered_map<std::string, Value>& variables,
                const std::unordered_map<std::string,
                std::function<void(const std::vector<Value>&)>>&builtInFunctions) const override {
        /* Doesn't do anything right now */
    }

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "IDENTIFIER (" + name + ")";
    }
};

// an expression representing 3*2 (or something of the like)
struct BinaryOperationNode : public ExpressionNode {
    std::unique_ptr<ExpressionNode> left;
    std::unique_ptr<ExpressionNode> right;
    char operation;

    BinaryOperationNode(std::unique_ptr<ExpressionNode> lhs, char op, std::unique_ptr<ExpressionNode> rhs)
        : left(std::move(lhs)), right(std::move(rhs)), operation(op) {}

    Value evaluate(const std::unordered_map<std::string, Value>& variables) const override {
        Value leftVal = left->evaluate(variables);
        Value rightVal = right->evaluate(variables);

        switch (operation) {;
            case '*': 
                return leftVal * rightVal;
            case '/': 
                return rightVal != 0 ? leftVal / rightVal : throw std::runtime_error("Division by zero");
            case '+': 
                return leftVal + rightVal;
            case '-': 
                return leftVal - rightVal;
        default: throw std::runtime_error("Unsupported operation");
        }
    }

    void visit(std::unordered_map<std::string, Value>& variables,
                const std::unordered_map<std::string,
                std::function<void(const std::vector<Value>&)>>&builtInFunctions) const override {
        /* Doesn't do anything right now */
    }

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "BIN-EXPR: (" + left->to_string() + " " +
            operation + " " + right->to_string() + ")";
    }
};

struct AssignmentNode : public ASTNode {
    std::string name;
    std::unique_ptr<ExpressionNode> expression;

    AssignmentNode(const std::string& name, std::unique_ptr<ExpressionNode> expr)
        : name(name), expression(std::move(expr)) {}

    void visit(std::unordered_map<std::string, Value>& variables,
        const std::unordered_map<std::string, 
        std::function<void(const std::vector<Value>&)>>& builtInFunctions) const override {
        if (variables.find(name) == variables.end()) 
            throw std::runtime_error("Variable '" + name + "' is not declared!");
        if (!variables[name].isMutable()) 
            throw std::runtime_error("Variable '" + name + "' is immutable!");
        variables[name] = expression->evaluate(variables);
    }

    std::string to_string(int indent = 0) const override {
        return std::string(indent, ' ') + "Assignment: " + name + " = (" + expression->to_string() + ")";
    }
};


// for expressions like let x = 54;
struct VariableDeclarationNode : public ASTNode {
    std::string name;
    bool mut;
    std::optional<std::unique_ptr<ExpressionNode>> expression; // Optional initializer

    VariableDeclarationNode(const std::string& name, std::optional<std::unique_ptr<ExpressionNode>> expr, bool mut)
        : name(name), expression(std::move(expr)), mut(mut) {}

    void visit(std::unordered_map<std::string, Value>& variables,
        const std::unordered_map<std::string, std::function<void(const std::vector<Value>&)>>& builtInFunctions) const override {
        if (expression) {
            Value value = (*expression)->evaluate(variables);
            value.setMutable(mut);
            variables[name] = std::move(value);
        }
        else {
            Value v; v.setMutable(mut);
            variables[name] = v;
        }
    }

    std::string to_string(int indent = 0) const override {
        std::string result = std::string(indent, ' ') + "Variable Declaration: " + name;
        if (expression) result += " = (" + (*expression)->to_string() + ")";
        return result;
    }
};


// all function calls for both built-in and user-defined functions
struct FunctionCallNode : public ASTNode {
    std::string functionName;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;

    FunctionCallNode(const std::string& name, std::vector<std::unique_ptr<ExpressionNode>> args)
        : functionName(name), arguments(std::move(args)) {}

    void visit(std::unordered_map<std::string, Value>& variables,
        const std::unordered_map<std::string, 
        std::function<void(const std::vector<Value>&)>>& builtInFunctions) const override {
        std::vector<Value> evaluatedArgs;
        for (const std::unique_ptr<ExpressionNode>& arg : arguments) 
            evaluatedArgs.push_back(arg->evaluate(variables));

        auto func = builtInFunctions.find(functionName);
        if (func != builtInFunctions.end()) func->second(evaluatedArgs);
        else throw std::runtime_error("Unknown function: " + functionName);
    }

    std::string to_string(int indent = 0) const override {
        std::string str = std::string(indent, ' ') + "FunctionCall: " + functionName + "(";
        for (const auto& arg : arguments) str += arg->to_string() + ", ";
        str.pop_back();
        str.pop_back();
        str += ")";
        return str;
    }
};