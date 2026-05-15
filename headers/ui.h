#pragma once

#include <iostream>
#include <string>
#include <limits>

class UI {
private:
  static constexpr int LINE_WIDTH = 60;

public:
  void displayHeader(const std::string &header) const;
  void printLine(char ch = '=') const;
  void pauseScreen() const;
};