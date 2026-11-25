#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"
namespace {

const char* const kHelpText =
    "支持的命令:\n"
    "  RUN - 开始执行程序\n"
    "  LIST - 列出当前所有的程序行\n"
    "  CLEAR - 清除当前所有的程序行\n"
    "  QUIT - 退出解释器\n"
    "  HELP - 打印帮助信息\n"
    "\n"
    "程序指令:\n"
    "  [行号] REM <注释> - 注释行\n"
    "  [行号] LET <变量> = <表达式> - 赋值语句\n"
    "  [行号] PRINT <表达式> - 打印表达式值\n"
    "  [行号] INPUT <变量> - 输入变量值\n"
    "  [行号] END - 结束程序\n"
    "  [行号] GOTO <行号> - 跳转到指定行\n"
    "  [行号] IF <表达式1> <操作符> <表达式2> THEN <行号> - 条件跳转\n"
    "  [行号] INDENT - 进入新的作用域块\n"
    "  [行号] DEDENT - 退出当前作用域块\n"
    "\n"
    "立即执行指令 (不带行号):\n"
    "  LET <变量> = <表达式>\n"
    "  PRINT <表达式>\n"
    "  INPUT <变量>\n"
    "  INDENT\n"
    "  DEDENT\n";

void printHelp() { std::cout << kHelpText; }

}  // namespace
int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // TODO: The main function.
      TokenStream tokens = lexer.tokenize(line);
      if (tokens.empty()) {
        throw BasicError("SYNTAX ERROR");
      }
      const Token* first = tokens.peek();
      switch (first->type) {
        case TokenType::RUN:
          program.run();
          continue;
        case TokenType::LIST:
          program.list();
          continue;
        case TokenType::CLEAR:
          program.clear();
          continue;
        case TokenType::QUIT:
          return 0;
        case TokenType::HELP:
          printHelp();
          continue;
        case TokenType::INDENT:
          program.indent();
          continue;
        case TokenType::DEDENT:
          program.dedent();
          continue;
        default:
          break;
      }
      ParsedLine parsed = parser.parseLine(tokens, line);
      if (parsed.getLine().has_value()) {
        const int lineNumber = *parsed.getLine();
        if (lineNumber <= 0) {
            throw BasicError("LINE NUMBER ERROR");
        }
        if (parsed.getStatement() == nullptr) {
            program.removeStmt(lineNumber);
        }
        else {
            program.addStmt(lineNumber, parsed.fetchStatement());
        }
      }
      else {
        program.execute(parsed.fetchStatement());
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}