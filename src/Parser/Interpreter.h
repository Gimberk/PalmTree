#pragma once

#include "AST.h"

#include <memory>

class Interpreter {
public:
    static void walkAST(const std::unique_ptr<ProgramNode>& program) {
        std::unordered_map<std::string, Value> variables;
        program->visit(variables, Lexer::BUILT_IN_FUNCTIONS);
    }
};