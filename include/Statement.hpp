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
    LetStatement(std::string source, std::string name, std::unique_ptr<Expression> expression);
    void execute(VarState& state, Program& program) const override;
  private:
    std::string name_;
    std::unique_ptr<Expression> expression_;
};
class PrintStatement : public Statement {
  public:
    PrintStatement(std::string source, std::unique_ptr<Expression> expression);
    void execute(VarState& state, Program& program) const override;
  private:
    std::unique_ptr<Expression> expression_;
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
    IfStatement(std::string source, std::unique_ptr<Expression> left, char op, std::unique_ptr<Expression> right, int targetLine);
    void execute(VarState& state, Program& program) const override;
  private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
    char op_;
    int targetLine_;
};
