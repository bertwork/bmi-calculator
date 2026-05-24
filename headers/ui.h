#pragma once

#include "bmi_service.h"
#include "colors.h"
#include "input_utility.h"
#include "user.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

class UI {
public:
  static constexpr int MAX_RECORDS = 500;
  static constexpr int LIST_DISPLAY_OFFSET = 1;

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

  void displayBMIResult(const User &user, int current = 0, int total = 0) const;
  void displayRecordList(const std::vector<const User *> &records) const;
  std::string promptLine(const std::string &prompt) const;
  std::string promptGender() const;
  int promptAge() const;
  void collectHeightWeight(double &heightCm, double &weightKg) const;
  bool confirm(const std::string &prompt) const;
  bool nameMatches(const std::string &name, const std::string &query) const;

private:
  static constexpr int LINE_WIDTH = 60;
  static constexpr int DECIMAL_PRECISION = 2;
  static constexpr int MENU_OPTION_MIN = 1;

  static constexpr int MIN_USER_AGE = 2;
  static constexpr int MAX_USER_AGE = 120;

  static constexpr int GENDER_OPTION_MIN = 1;
  static constexpr int GENDER_OPTION_MAX = 3;

  enum class GenderChoice : int { Male = 1, Female = 2, PreferNotToSay = 3 };

  static constexpr int UNIT_OPTION_MIN = 1;
  static constexpr int UNIT_OPTION_MAX = 2;

  enum class HeightUnit : int { Centimeters = 1, Feet = 2 };
  enum class WeightUnit : int { Kilograms = 1, Pounds = 2 };

  static constexpr double MIN_HEIGHT_CM = 50.0;
  static constexpr double MAX_HEIGHT_CM = 272.0;
  static constexpr double MIN_HEIGHT_FEET = 1.6;
  static constexpr double MAX_HEIGHT_FEET = 9.0;

  static constexpr double MIN_WEIGHT_KG = 2.0;
  static constexpr double MAX_WEIGHT_KG = 500.0;
  static constexpr double MIN_WEIGHT_LB = 4.0;
  static constexpr double MAX_WEIGHT_LB = 1102.0;

  static inline const std::vector<std::string> menu = {
      "Quick BMI Calculation", "Save BMI Record", "View All Records",
      "Search Record",         "Delete Record",   "Exit"};

  void displayRecordLine(int listIndex, const User &user) const;
  void collectHeight(double &heightCm) const;
  void collectWeight(double &weightKg) const;
};
