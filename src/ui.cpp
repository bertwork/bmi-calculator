#include "ui.h"

UI::UI(FileManager &fm) : fileManager(fm) {}

void UI::displayHeader(const std::string &header) const {
  printLine();

  int padding = (LINE_WIDTH - (int)header.length()) / 2;
  if (padding < 0)
    padding = 0;

  std::cout << std::string(padding, ' ') << header << "\n";

  printLine();
}

void UI::displayMenu() const {
  displayHeader("BMI CALCULATOR SYSTEM");

  std::cout << "Total Records: " << fileManager.getRecordCount() << " / " << MAX_RECORDS << "\n\n";

  std::cout << "MENU OPTIONS:\n";

  for (size_t i = 0; i < menu.size(); i++) {
    std::cout << "[" << i + 1 << "] " << menu[i] << "\n";
  }
}

void UI::printLine(char ch) const {
  std::cout << std::string(LINE_WIDTH, ch) << "\n";
}

void UI::pauseScreen() const {
  printLine();
  std::cout << "Press Enter to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}