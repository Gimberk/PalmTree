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
    std::string code = "let x = 5%4;\nprint(x*100);";
    std::vector<Token> tokens = Lexer::tokenize(code);

    for (auto& token : tokens) std::cout << token.toString() << '\n';

    Parser parser(tokens);
    std::unique_ptr<ProgramNode> ast = parser.parse();

    std::cout << "\n" << ast->to_string() << '\n';
    for (std::unique_ptr<ASTNode>& node : ast->statements) {
        std::cout << node->to_string() << "\n";
    }

    std::cout << "\nCode:\n";
    std::cout << code << "\n\n";
    std::cout << "Output:\n";
    Interpreter::walkAST(ast);
}