#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
class RemStatement : public Statement {
  public:
    explicit RemStatement(std::string source);
    ~RemStatement() override = default;
    void execute(VarState& state, Program& program) const override;
};
class LetStatement : public Statement {
  public:
    LetStatement(std::string source, std::string name, Expression* expression);
    ~LetStatement() override;
    void execute(VarState& state, Program& program) const override;
  private:
    std::string name_;
    Expression* expression_;
};
class PrintStatement : public Statement {
  public:
    PrintStatement(std::string source, Expression* expression);
    ~PrintStatement() override;
    void execute(VarState& state, Program& program) const override;
  private:
    Expression* expression_;
};
class InputStatement : public Statement {
  public:
    InputStatement(std::string source, std::string name);
    ~InputStatement() override = default;
    void execute(VarState& state, Program& program) const override;
  private:
    std::string name_;
};
class EndStatement : public Statement {
  public:
    explicit EndStatement(std::string source);
    ~EndStatement() override = default;
    void execute(VarState& state, Program& program) const override;
};
class GotoStatement : public Statement {
  public:
    GotoStatement(std::string source, int targetLine);
    ~GotoStatement() override = default;
    void execute(VarState& state, Program& program) const override;
  private:
    int targetLine_;
};
class IfStatement : public Statement {
  public:
    IfStatement(std::string source, Expression* left, char op, Expression* right, int targetLine);
    ~IfStatement() override;
    void execute(VarState& state, Program& program) const override;
  private:
    Expression* left_;
    Expression* right_;
    char op_;
    int targetLine_;
};
