#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class VarState {
 public:
  VarState();
  void setValue(const std::string& name, int value);
  int getValue(const std::string& name) const;
  void clear();
  void enterScope();
  void leaveScope();
  void resetScope() noexcept;
 private:
  std::vector<std::unordered_map<std::string, int>> scopes_;
};
