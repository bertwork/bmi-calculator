#include "app.h"

App::App(const std::string &db_folder)
    : file_manager(db_folder), ui(), bmi_service() {
  file_manager.init_database();
}

void App::run() {
  int menu_choice;
  const int EXIT = static_cast<int>(UI::MenuOption::EXIT);

  do {
    ui.displayMenu(file_manager.getRecordCount());
    ui.menuChoice(menu_choice);
    handleMenuChoice(static_cast<UI::MenuOption>(menu_choice));

  } while (menu_choice != EXIT);
}

void App::handleMenuChoice(UI::MenuOption choice) {
  switch (choice) {
  case UI::MenuOption::QUICK_BMI:
    quickCalculate();
    ui.pauseScreen();
    break;
  case UI::MenuOption::SAVE_RECORD:
    saveRecord();
    ui.pauseScreen();
    break;
  case UI::MenuOption::VIEW_RECORDS:
    viewRecords();
    ui.pauseScreen();
    break;
  case UI::MenuOption::SEARCH:
    searchRecord();
    ui.pauseScreen();
    break;
  case UI::MenuOption::DELETE:
    deleteRecord();
    ui.pauseScreen();
    break;
  case UI::MenuOption::EXIT:
    ui.displayHeader("Goodbye!");
    break;
  }
}

void App::quickCalculate() {
  ui.printLine('-');
  std::cout << "QUICK BMI CALCULATION SELECTED\n";
}

void App::saveRecord() {
  ui.printLine('-');
  std::cout << "SAVE BMI RECORD SELECTED\n";
}

void App::viewRecords() {
  ui.printLine('-');
  std::cout << "VIEW ALL RECORDS SELECTED\n";
}
void App::searchRecord() {
  ui.printLine('-');
  std::cout << "SEARCH RECORD SELECTED\n";
}
void App::deleteRecord() {
  ui.printLine('-');
  std::cout << "DELETE RECORD SELECTED\n";
}