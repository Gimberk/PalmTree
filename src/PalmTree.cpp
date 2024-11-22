#include <cstdio>

#include "Lexer/Lexer.h"
#include "Parser/Interpreter.h"
#include "Parser/Parser.h"

/*
 =======================================================
    ------------------- WARNING ---------------------
 =======================================================
              i hate Abstract Syntax Trees
*/

int main() {
  std::string code =
      "let a = () => 4; print(a());";  // doesn't allow for sub 2 argument
                                       // definitions cuz bad
  //  std::string code = "let x = 4 + 4;";
  //  std::string code = "let x = 5 |> increment; print(x);";
  std::vector<Token> tokens = Lexer::tokenize(code);

  for (auto& token : tokens) std::cout << token.to_string() << '\n';
  std::cout << '\n';

  Parser parser(tokens);
  std::unique_ptr<ProgramNode> ast = parser.parse();

  std::cout << "\n" << ast->to_string() << '\n';
  for (std::unique_ptr<ASTNode>& node : ast->statements)
    std::cout << node->to_string() << "\n";

  std::cout << "Code:\n";
  std::cout << code << "\n\n";
  std::cout << "Output:\n";
  Interpreter::walkAST(ast);
}
