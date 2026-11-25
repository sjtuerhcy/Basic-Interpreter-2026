// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"
#include "utils/Error.hpp"

#include<iostream>

Program::Program() : recorder_(), scopes_(), programCounter_(0), programEnd_(false) {};

void Program::addStmt(int line, std::unique_ptr<Statement> stmt) {
  recorder_.add(line, std::move(stmt));
}

void Program::removeStmt(int line) {
  recorder_.remove(line);
}

void Program::run() {
  programEnd_ = false;
  programCounter_ = recorder_.nextLine(0);
  try {
    while (programCounter_ != -1 && !programEnd_) {
      const int cur = programCounter_;
      programCounter_ = recorder_.nextLine(cur);
      const Statement* stmt = recorder_.get(cur);
      if (stmt == nullptr) {
        break;
      }
      stmt->execute(scopes_, *this);
    }
  } catch (const BasicError& e) {
    std::cout << e.message() << "\n";
  }
  resetAfterRun();
}

void Program::list() const {
  recorder_.printLines();
}

void Program::clear() {
  recorder_.clear();
  scopes_.clear();
}

void Program::execute(std::unique_ptr<Statement> stmt) {
  if (stmt != nullptr) {
    stmt->execute(scopes_, *this);
  }
}

int Program::getPC() const noexcept {
  return programCounter_;
}

void Program::changePC(int line) {
  if (!recorder_.hasLine(line)) {
    throw BasicError("LINE NUMBER ERROR");
  }
  programCounter_ = line;
}

void Program::programEnd() {
  programEnd_ = true;
}

void Program::indent() {
  scopes_.enterScope();
}

void Program::dedent() {
  scopes_.leaveScope();
}

void Program::resetAfterRun() noexcept {
  programCounter_ = 0;
  programEnd_ = false;
}