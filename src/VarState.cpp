#include "VarState.hpp"

#include <algorithm>

#include "utils/Error.hpp"

VarState::VarState() : scopes_(1) {};

void VarState::setValue(const std::string& name, int value) {
  scopes_.back()[name] = value;
}

int VarState::getValue(const std::string& name) const {
  for (auto it = scopes_.rbegin(); it != scopes_.rend(); it++) {
    auto found = it->find(name);
    if (found != it->end()) {
        return found->second;
    }
  }
  throw BasicError("VARIABLE NOT DEFINED");
}

void VarState::clear() { 
  scopes_.clear(); 
  scopes_.resize(1);
}

void VarState::enterScope() {
  scopes_.push_back(std::unordered_map<std::string, int>());
}

void VarState::leaveScope() {
  if (scopes_.size() <= 1) {
    throw BasicError("SCOPE UNDERFLOW");
  }
  scopes_.pop_back();
}

void VarState::resetScope() noexcept {
  scopes_.resize(1);
}