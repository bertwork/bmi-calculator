#pragma once

#include "file_manager.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

class UI {
private:
  FileManager &fileManager;
  static constexpr int LINE_WIDTH = 60;
  static constexpr int MAX_RECORDS = 500;

  static inline const std::vector<std::string> menu = {
      "Quick BMI Calculation", "Save BMI Record", "View All Records",
      "Search Record",         "Delete Record",   "Exit"};

public:
  UI(FileManager &f_manager);
  void displayHeader(const std::string &header) const;
  void displayMenu() const;
  void printLine(char ch = '=') const;
  void pauseScreen() const;
};