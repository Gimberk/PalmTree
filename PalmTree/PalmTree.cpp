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
    std::string code = "let x1 mut;print(x1);\nx1=4;print(x1);";
    std::vector<Token> tokens = Lexer::tokenize(code);

    for (auto& token : tokens) std::cout << token.to_string() << '\n';
    std::cout << '\n';

    Parser parser(tokens);
    std::unique_ptr<ProgramNode> ast = parser.parse();

    std::cout << "Code:\n";
    std::cout << code << "\n\n";
    std::cout << "Output:\n";
    Interpreter::walkAST(ast);
}