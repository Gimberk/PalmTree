#include <cstdio>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/Interpreter.h"

/*
 =======================================================
    ------------------- WARNING ---------------------
 =======================================================
              i hate Abstract Syntax Trees
*/

int main()
{
    std::string code = "let x1 = 1 + 3 * (1 + 2);\nprint(x1);";
    std::vector<Token> tokens = Lexer::tokenize(code);

    for (Token& token : tokens) std::cout << token.to_string() << '\n';

    Parser parser(tokens);
    std::unique_ptr<ProgramNode> ast = parser.parse();

    std::cout << "\n" << ast->to_string() << '\n';
    for (std::unique_ptr<ASTNode>& node : ast->statements) 
        std::cout << node->to_string() << "\n";

    std::cout << "\nCode:\n";
    std::cout << code << "\n\n";
    std::cout << "Output:\n";
    Interpreter::walkAST(ast);
}