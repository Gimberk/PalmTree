#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Types/Value.h"

struct ASTNode {
  virtual std::string to_string(
      int indent = 0) const = 0;  // for debug purposes to visualize the AST
  virtual Value visit(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const = 0;
  virtual ~ASTNode() = default;
};

struct ProgramNode : public ASTNode {
 public:
  std::vector<std::unique_ptr<ASTNode>> statements;

  ProgramNode(std::vector<std::unique_ptr<ASTNode>> stmts)
      : statements(std::move(stmts)) {}

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    for (const auto& stmt : statements)
      stmt->visit(variables, builtInFunctions);
    return Value();
  }

  std::string to_string(int indent = 0) const override {
    return std::string(indent, ' ') + "PROGRAM NODE";
  }
};

// parent class for expressions of all types
struct ExpressionNode : public ASTNode {
  virtual Value evaluate(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const = 0;
  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override = 0;
};

// number literals like 5
struct NumberNode : public ExpressionNode {
  Value value;

  NumberNode(Value val) : value(val) {}

  Value evaluate(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const override {
    return value;
  }

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    /* Doesn't do anything right now */
    return Value();
  }

  std::string to_string(int indent = 0) const override {
    return std::string(indent, ' ') + "NUMBER-LIT (" + value.to_string() + ")";
  }
};

// for identifiers like print(x); (x is considered a variable-node)
struct VariableNode : public ExpressionNode {
  std::string name;
  VariableNode(const std::string& name) : name(name) {}

  Value evaluate(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const override {
    auto it = variables.find(name);
    if (it != variables.end())
      return it->second;
    else {
      std::cout << "Undefined variable: " + name << "\n";
      throw std::runtime_error("Undefined variable: " + name);
    }
  }

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    /* Doesn't do anything right now */
    return Value();
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

  BinaryOperationNode(std::unique_ptr<ExpressionNode> lhs, char op,
                      std::unique_ptr<ExpressionNode> rhs)
      : left(std::move(lhs)), right(std::move(rhs)), operation(op) {}

  Value evaluate(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const override {
    Value leftVal = left->evaluate(variables, builtInFunctions);
    Value rightVal = right->evaluate(variables, builtInFunctions);

    switch (operation) {
      ;
      case '*':
        return leftVal * rightVal;
      case '/':
        return rightVal != 0 ? leftVal / rightVal
                             : throw std::runtime_error("Division by zero");
      case '+':
        return leftVal + rightVal;
      case '-':
        return leftVal - rightVal;
      default:
        throw std::runtime_error("Unsupported operation");
    }
  }

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    /* Doesn't do anything right now */
    return Value();
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

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    if (variables.find(name) == variables.end())
      throw std::runtime_error("Variable '" + name + "' is not declared!");
    if (!variables[name].isMutable())
      throw std::runtime_error("Variable '" + name + "' is immutable!");
    variables[name] = expression->evaluate(variables, builtInFunctions);
    return Value();
  }

  std::string to_string(int indent = 0) const override {
    return std::string(indent, ' ') + "Assignment: " + name + " = (" +
           expression->to_string() + ")";
  }
};

struct LambdaNode : public ExpressionNode,
                    public std::enable_shared_from_this<LambdaNode> {
 public:
  std::string functionName;
  std::vector<std::string> arguments;
  std::unique_ptr<ExpressionNode> body;

  LambdaNode(std::string functionName, std::vector<std::string> arguments,
             std::unique_ptr<ExpressionNode> body)
      : functionName(functionName),
        arguments(arguments),
        body(std::move(body)) {}

  LambdaNode(std::unique_ptr<LambdaNode> ptr)
      : functionName(ptr->functionName),
        arguments(ptr->arguments),
        body(std::move(ptr->body)) {}

  Value evaluate(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const override {
    return Value(shared_from_this());
  }

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    Value exprResult = body->evaluate(variables, builtInFunctions);
    for (std::string arg : arguments)
      variables.erase(arg);  // very safe way to clean out local scope
    return exprResult;
  }

  std::string to_string(int indent = 0) const override { return ""; }
};

// for expressions like let x = 54;
struct VariableDeclarationNode : public ASTNode {
  std::string name;
  std::optional<std::unique_ptr<ExpressionNode>> expression;
  std::optional<std::shared_ptr<LambdaNode>> lambdaExpr;
  bool mut;

  VariableDeclarationNode(const std::string& name,
                          std::optional<std::unique_ptr<ExpressionNode>> expr,
                          std::optional<std::shared_ptr<LambdaNode>> lambdaExpr,
                          bool mut)
      : name(name),
        expression(std::move(expr)),
        lambdaExpr(std::move(lambdaExpr)),
        mut(mut) {}

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    if (variables.find(name) != variables.end())
      throw std::runtime_error("Variable with identifier already exists!");
    Value value;
    if (!expression.has_value() && !lambdaExpr.has_value())
      value = Value();
    else if (expression.has_value() && !lambdaExpr.has_value())
      value = (*expression)->evaluate(variables, builtInFunctions);
    else
      value = Value((*lambdaExpr));
    value.setMutable(mut);
    variables[name] = value;
    return value;
  }

  std::string to_string(int indent = 0) const override {
    std::string result =
        std::string(indent, ' ') + "Variable Declaration: " + name;
    if (expression) result += " = (" + (*expression)->to_string() + ")";
    return result;
  }
};

// all function calls for both built-in and user-defined functions
struct FunctionCallNode : public ExpressionNode {
  std::string functionName;
  std::vector<std::unique_ptr<ExpressionNode>> arguments;

  FunctionCallNode(const std::string& name,
                   std::vector<std::unique_ptr<ExpressionNode>> args)
      : functionName(name), arguments(std::move(args)) {}

  Value evaluate(
      std::unordered_map<std::string, Value>& variables,
      const std::unordered_map<std::string,
                               std::function<Value(const std::vector<Value>&)>>&
          builtInFunctions) const override {
    std::vector<Value> evaluatedArgs;
    for (const std::unique_ptr<ExpressionNode>& arg : arguments)
      evaluatedArgs.push_back(arg->evaluate(variables, builtInFunctions));

    // for built-in functions
    auto func = builtInFunctions.find(functionName);
    if (func != builtInFunctions.end()) return func->second(evaluatedArgs);

    // for lambda functions
    for (auto& var : variables) {
      std::cout << var.first << '\n';
      std::cout << var.second.isLambda() << '\n';
      if (functionName != var.first || !var.second.isLambda()) continue;
      auto lambda = var.second.asLambda();
      // this means that a global var already exists with the same name as the
      // arg
      for (std::string arg : lambda->arguments)
        if (variables.find(arg) != variables.end())
          throw std::runtime_error("Variable with identifier already exists!");
      if (evaluatedArgs.size() != lambda->arguments.size())
        throw std::runtime_error("Input count mismatch");
      for (int i = 0; i < evaluatedArgs.size(); i++)
        variables[lambda->arguments[i]] = evaluatedArgs[i];
      return lambda->visit(variables, builtInFunctions);
    }
    throw std::runtime_error("Unknown function: " + functionName);
  }

  Value visit(std::unordered_map<std::string, Value>& variables,
              const std::unordered_map<
                  std::string, std::function<Value(const std::vector<Value>&)>>&
                  builtInFunctions) const override {
    return evaluate(variables, builtInFunctions);
  }

  std::string to_string(int indent = 0) const override {
    std::string str =
        std::string(indent, ' ') + "FunctionCall: " + functionName + "(";
    for (const auto& arg : arguments) str += arg->to_string() + ", ";
    str.pop_back();
    str.pop_back();
    str += ")";
    return str;
  }
};
