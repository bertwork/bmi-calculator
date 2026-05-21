#include "ui.h"

#include <algorithm>
#include <cctype>
#include <iomanip>

namespace {

constexpr char CONFIRM_YES = 'y';
constexpr char CONFIRM_NO = 'n';

} // namespace

void UI::displayHeader(const std::string &header) const {
  printLine();

  int padding = (LINE_WIDTH - static_cast<int>(header.length())) / 2;
  if (padding < 0) {
    padding = 0;
  }

  std::cout << std::string(padding, ' ') << header << "\n";
  printLine();
}

void UI::displayMenu(int currentRecordCount) const {
  displayHeader("BMI CALCULATOR SYSTEM");

  std::cout << "Total Records: " << currentRecordCount << " / "
            << MAX_RECORDS << "\n\n";
  std::cout << "MENU OPTIONS:\n";

  for (size_t i = 0; i < menu.size(); ++i) {
    std::cout << "[" << i + LIST_DISPLAY_OFFSET << "] " << menu[i] << "\n";
  }

  std::cout << "\n";
}

void UI::printLine(char ch) const {
  std::cout << std::string(LINE_WIDTH, ch) << "\n";
}

void UI::pauseScreen() const {
  std::cout << "\n";
  printLine();
  std::cout << "Press Enter to continue...";
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void UI::menuChoice(int &choice) const {
  const int maxOption = static_cast<int>(menu.size());
  const std::string prompt =
      "Select an option (" + std::to_string(MENU_OPTION_MIN) + "-" +
      std::to_string(maxOption) + "): ";
  getInput(prompt, choice, MENU_OPTION_MIN, maxOption);
}

void UI::displayBMIResult(const User &user) const {
  displayHeader("BMI RESULT");

  std::cout << std::fixed << std::setprecision(DECIMAL_PRECISION);
  std::cout << "  Name     : " << user.get_name() << "\n";
  std::cout << "  Gender   : " << user.get_gender() << "\n";
  std::cout << "  Age      : " << user.get_age() << "\n";
  std::cout << "  Height   : " << user.get_height() << " cm\n";
  std::cout << "  Weight   : " << user.get_weight() << " kg\n";
  printLine('-');
  std::cout << "  BMI      : " << user.get_bmi() << "\n";
  std::cout << "  Category : " << user.get_category() << "\n";
  std::cout << "  Advice   : " << user.get_advice() << "\n";
  std::cout << "  Risk     : " << user.get_risk() << "\n";
}

void UI::displayRecordLine(int listIndex, const User &user) const {
  std::cout << "  [" << listIndex << "] ID: " << user.get_id() << " | "
            << user.get_name() << " | " << user.get_gender()
            << " | Age: " << user.get_age() << " | BMI: " << std::fixed
            << std::setprecision(DECIMAL_PRECISION) << user.get_bmi() << " | "
            << user.get_category() << "\n";
}

void UI::displayRecordList(const std::vector<User *> &records) const {
  printLine('-');
  for (size_t i = 0; i < records.size(); ++i) {
    if (records[i] != nullptr) {
      displayRecordLine(static_cast<int>(i + LIST_DISPLAY_OFFSET), *records[i]);
      printLine('-');
    }
  }
}

std::string UI::promptLine(const std::string &prompt) const {
  std::string value;
  while (true) {
    std::cout << prompt;
    std::getline(std::cin, value);
    if (!value.empty()) {
      return value;
    }
    std::cout << "  Input cannot be empty. Please try again.\n";
  }
}

std::string UI::promptGender() const {
  printLine('-');
  std::cout << "Select gender:\n";
  std::cout << "[" << static_cast<int>(GenderChoice::Male) << "] Male\n";
  std::cout << "[" << static_cast<int>(GenderChoice::Female) << "] Female\n";
  std::cout << "[" << static_cast<int>(GenderChoice::PreferNotToSay)
            << "] Prefer not to say\n";
  printLine('-');

  int choice = 0;
  getInput("Enter gender option (" + std::to_string(GENDER_OPTION_MIN) + "-" +
               std::to_string(GENDER_OPTION_MAX) + "): ",
           choice, GENDER_OPTION_MIN, GENDER_OPTION_MAX);
  printLine('-');

  switch (static_cast<GenderChoice>(choice)) {
  case GenderChoice::Male:
    return "Male";
  case GenderChoice::Female:
    return "Female";
  default:
    return "Prefer not to say";
  }
}

int UI::promptAge() const {
  int age = 0;
  getInput("Enter age (" + std::to_string(MIN_USER_AGE) + "-" +
               std::to_string(MAX_USER_AGE) + "): ",
           age, MIN_USER_AGE, MAX_USER_AGE);
  return age;
}

void UI::collectHeightWeight(double &heightCm, double &weightKg) const {
  collectHeight(heightCm);
  collectWeight(weightKg);
}

bool UI::confirm(const std::string &prompt) const {
  while (true) {
    std::cout << prompt;
    std::string answer;
    std::getline(std::cin, answer);

    if (answer.empty()) {
      continue;
    }

    const char c = static_cast<char>(
        std::tolower(static_cast<unsigned char>(answer[0])));
    if (c == CONFIRM_YES) {
      return true;
    }
    if (c == CONFIRM_NO) {
      return false;
    }

    std::cout << "  Please enter y or n.\n";
  }
}

bool UI::nameMatches(const std::string &name, const std::string &query) const {
  if (query.empty()) {
    return true;
  }

  std::string lowerName = name;
  std::string lowerQuery = query;
  std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return lowerName.find(lowerQuery) != std::string::npos;
}

void UI::collectHeight(double &heightCm) const {
  printLine('-');
  std::cout << "Height unit:\n";
  std::cout << "[" << static_cast<int>(HeightUnit::Centimeters)
            << "] Centimeters\n";
  std::cout << "[" << static_cast<int>(HeightUnit::Feet) << "] Feet\n";
  printLine('-');
  int unit = 0;
  getInput("Enter unit option (" + std::to_string(UNIT_OPTION_MIN) + "-" +
               std::to_string(UNIT_OPTION_MAX) + "): ",
           unit, UNIT_OPTION_MIN, UNIT_OPTION_MAX);
  printLine('-');

  auto fmtDouble = [](double val, int precision = 0) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << val;
    return oss.str();
  };

  if (static_cast<HeightUnit>(unit) == HeightUnit::Centimeters) {
    getInput("Enter height in cm (" + fmtDouble(MIN_HEIGHT_CM) + "-" +
                 fmtDouble(MAX_HEIGHT_CM) + "): ",
             heightCm, MIN_HEIGHT_CM, MAX_HEIGHT_CM);
  } else {
    double feet = 0.0;
    getInput("Enter height in feet (" + fmtDouble(MIN_HEIGHT_FEET, 1) + "-" +
                 fmtDouble(MAX_HEIGHT_FEET) + "): ",
             feet, MIN_HEIGHT_FEET, MAX_HEIGHT_FEET);
    heightCm = feet * BMIService::FEET_TO_METER * BMIService::CM_TO_METERS;
  }
}

void UI::collectWeight(double &weightKg) const {
  printLine('-');
  std::cout << "Weight unit:\n";
  std::cout << "[" << static_cast<int>(WeightUnit::Kilograms)
            << "] Kilograms\n";
  std::cout << "[" << static_cast<int>(WeightUnit::Pounds) << "] Pounds\n";
  printLine('-');

  int unit = 0;
  getInput("Enter unit option (" + std::to_string(UNIT_OPTION_MIN) + "-" +
               std::to_string(UNIT_OPTION_MAX) + "): ",
           unit, UNIT_OPTION_MIN, UNIT_OPTION_MAX);
  printLine('-');

  if (static_cast<WeightUnit>(unit) == WeightUnit::Kilograms) {
    getInput("Enter weight in kg (" +
                 std::to_string(static_cast<int>(MIN_WEIGHT_KG)) + "-" +
                 std::to_string(static_cast<int>(MAX_WEIGHT_KG)) + "): ",
             weightKg, MIN_WEIGHT_KG, MAX_WEIGHT_KG);
  } else {
    double pounds = 0.0;
    getInput("Enter weight in pounds (" +
                 std::to_string(static_cast<int>(MIN_WEIGHT_LB)) + "-" +
                 std::to_string(static_cast<int>(MAX_WEIGHT_LB)) + "): ",
             pounds, MIN_WEIGHT_LB, MAX_WEIGHT_LB);
    weightKg = BMIService::convertMass(pounds, true);
  }
  printLine('-');
}
