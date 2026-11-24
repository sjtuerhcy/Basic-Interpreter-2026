// TODO: Imply interfaces declared in the Recorder.hpp.
#include "Recorder.hpp"

#include<iostream>

void Recorder::add(int line, std::unique_ptr<Statement> stmt) {
  lines_[line] = std::move(stmt);
}

void Recorder::remove(int line) {
  lines_.erase(line);
}

const Statement* Recorder::get(int line) const noexcept {
  auto it = lines_.find(line);
  if (it != lines_.end()) {
    return it->second.get();
  }
  return nullptr;
}

bool Recorder::hasLine(int line) const noexcept {
  auto it = lines_.find(line);
  if (it != lines_.end()) {
    return true;
  }
  return false;
}

void Recorder::clear() noexcept {
  lines_.clear();
}

void Recorder::printLines() const {
  for (auto it = lines_.begin(); it != lines_.end(); it++) {
    std::cout << it->second->text() << '\n';
  }
}

int Recorder::nextLine(int line) const noexcept {
  auto it = lines_.upper_bound(line);
  if (it != lines_.end()) {
    return it->first;
  }
  return -1;
}