#include <iostream>
#include <cstdio>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

int main()
{
    std::cout << "Hello World!\n\n";
    std::string test = "let x = 5; print(x);";
    std::vector<Token> tokens = Lexer::tokenize(test);
    for (auto& token : tokens) std::cout << token.toString() << '\n';

    Parser parser(tokens);
    std::vector<std::unique_ptr<ASTNode>> ast = parser.parse();

    for (std::unique_ptr<ASTNode>& node : ast) {
        std::cout << node->to_string() << "\n";
    }
}