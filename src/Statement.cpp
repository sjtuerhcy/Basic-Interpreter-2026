#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.

RemStatement::RemStatement(std::string source) : Statement(std::move(source)) {}
void RemStatement::execute(VarState&, Program&) const{}

LetStatement::LetStatement(std::string source, std::string name, std::unique_ptr<Expression> expression)
    : Statement(std::move(source)), name_(std::move(name)), expression_(std::move(expression)) {}

void LetStatement::execute(VarState& state, Program&) const {
  state.setValue(name_, expression_->evaluate(state));
}

PrintStatement::PrintStatement(std::string source, std::unique_ptr<Expression> expression)
    : Statement(std::move(source)), expression_(std::move(expression)) {}
void PrintStatement::execute(VarState& state, Program&) const{
  std::cout << expression_->evaluate(state) << '\n';
}

InputStatement::InputStatement(std::string source, std::string name)
    : Statement(std::move(source)), name_(std::move(name)) {}
void InputStatement::execute(VarState& state, Program&) const {
  std::string line;
  while (true) {
    std::cout << " ? ";
    if (!std::getline(std::cin, line)) {
      return;
    }
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    std::size_t i = 0;
    bool negative = false;
    if (i < line.size() && (line[i] == '+' || line[i] == '-')) {
      negative = (line[i] == '-');
      i++;
    }
    bool valid = (i < line.size());
    long long value = 0;
    for (; valid && i < line.size(); i++) {
      if (line[i] < '0' || line[i] > '9') {
        valid = false;
      }
      else {
        value = value * 10 + (line[i] - '0');
        if (value > 2147483648LL) {
          valid = false;
        }
      }
    }
    if (negative) {
      value = -value;
    }
    if (value < -2147483648LL || value > 2147483647LL) {
      valid = false;
    }
    if (valid) {
      state.setValue(name_, static_cast<int>(value));
      return;
    }
    std::cout << "INVALID NUMBER\n";
  }
}

EndStatement::EndStatement(std::string source) : Statement(std::move(source)) {}
void EndStatement::execute(VarState&, Program& program) const {
  program.programEnd();
}

GotoStatement::GotoStatement(std::string source, int targetLine)
    : Statement(std::move(source)), targetLine_(targetLine) {}
void GotoStatement:: execute(VarState&, Program& program) const {
  program.changePC(targetLine_);
}

IfStatement::IfStatement(std::string source, std::unique_ptr<Expression> left, char op, std::unique_ptr<Expression> right, int targetLine)
    : Statement(std::move(source)), left_(std::move(left)), right_(std::move(right)), op_(op), targetLine_(targetLine) {}

void IfStatement::execute(VarState& state, Program& program) const {
  const int leftValue = left_->evaluate(state);
  const int rightValue = right_->evaluate(state);
  bool condition = false;
  switch (op_) {
    case '=':
      condition = (leftValue == rightValue);
      break;
    case '<':
      condition = (leftValue < rightValue);
      break;
    case '>':
      condition = (leftValue > rightValue);
      break;
    default:
      throw BasicError("SYNTAX ERROR");
  }
  if (condition) {
    program.changePC(targetLine_);
  }
}