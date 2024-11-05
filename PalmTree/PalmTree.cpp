#include <iostream>
#include <cstdio>

#include "Lexer/Lexer.h"

int main()
{
    std::cout << "Hello World!\n\n";
    std::string test = "let x = 445;\nlet y = x + 3;\nprint(y);";
    std::vector<Token> tokens = Lexer::tokenize(test);
    for (auto& token : tokens) std::cout << token.toString() << '\n';
}