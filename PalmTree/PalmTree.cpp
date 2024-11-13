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
    std::string code = "let add = (x, y) => x + y;\nadd(4, 3);";
    //std::string code = "let x = 4 + 4;";
    // let x = 5 |> increment;
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