#include "app.h"

App::App(const std::string &db_folder)
    : file_manager(db_folder), ui(), bmi_service(file_manager, ui) {
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
    bmi_service.quickCalculate();
    ui.pauseScreen();
    break;
  case UI::MenuOption::SAVE_RECORD:
    bmi_service.saveRecord();
    ui.pauseScreen();
    break;
  case UI::MenuOption::VIEW_RECORDS:
    bmi_service.viewRecords();
    ui.pauseScreen();
    break;
  case UI::MenuOption::SEARCH:
    bmi_service.searchRecord();
    ui.pauseScreen();
    break;
  case UI::MenuOption::DELETE:
    bmi_service.deleteRecord();
    ui.pauseScreen();
    break;
  case UI::MenuOption::EXIT:
    ui.displayHeader("Goodbye!");
    break;
  }
}