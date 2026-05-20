#pragma once

#include "file_manager.h"
#include "input_utility.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

class UI {
private:
  static constexpr int LINE_WIDTH = 60;
  static constexpr int MAX_RECORDS = 500;

  static inline const std::vector<std::string> menu = {
      "Quick BMI Calculation", "Save BMI Record", "View All Records",
      "Search Record",         "Delete Record",   "Exit"};

public:
  enum class MenuOption {
    QUICK_BMI = 1,
    SAVE_RECORD,
    VIEW_RECORDS,
    SEARCH,
    DELETE,
    EXIT
  };

  UI() = default;
  
  void displayHeader(const std::string &header) const;
  void displayMenu(int currentRecordCount) const;
  void printLine(char ch = '=') const;
  void pauseScreen() const;
  void menuChoice(int &choice) const;
};