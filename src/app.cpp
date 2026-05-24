#include "app.h"

App::App(const std::string &db_folder) : file_manager(db_folder), ui() {
  file_manager.init_database();
}

void App::run() {
  int menu_choice = 0;
  const int exitChoice = static_cast<int>(UI::MenuOption::EXIT);

  do {
    ui.displayMenu(file_manager.getRecordCount());
    ui.menuChoice(menu_choice);
    handleMenuChoice(static_cast<UI::MenuOption>(menu_choice));
  } while (menu_choice != exitChoice);
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
  double heightCm = 0.0;
  double weightKg = 0.0;
  ui.collectHeightWeight(heightCm, weightKg);

  User user;
  user.set_name(ANONYMOUS_NAME);
  user.set_gender(UNKNOWN_GENDER);
  user.set_age(QUICK_CALC_AGE);
  user.set_height(heightCm);
  user.set_weight(weightKg);
  BMIService::applyToUser(user);

  ui.displayBMIResult(user);
}

void App::saveRecord() {
  if (file_manager.getRecordCount() >= UI::MAX_RECORDS) {
    std::cout << "Maximum record limit reached (" << UI::MAX_RECORDS
              << "). Cannot save more records.\n";
    return;
  }

  User user;
  user.set_name(ui.promptLine("Enter name: "));
  user.set_gender(ui.promptGender());
  user.set_age(ui.promptAge());

  double heightCm = 0.0;
  double weightKg = 0.0;

  ui.collectHeightWeight(heightCm, weightKg);
  user.set_height(heightCm);
  user.set_weight(weightKg);
  BMIService::applyToUser(user);

  ui.displayBMIResult(user);
  file_manager.create(user);
}

void App::viewRecords() {
  const std::vector<const User *> records = file_manager.read_all();

  if (records.empty()) {
    std::cout << "No records found.\n";
    return;
  }

  ui.displayHeader("ALL RECORDS");
  ui.displayRecordList(records);
}

void App::searchRecord() {
  const std::vector<const User *> records = file_manager.read_all();

  if (records.empty()) {
    std::cout << "No records found.\n";
    return;
  }

  const std::string query = ui.promptLine("Enter name to search: ");

  int totalMatches = 0;
  for (const User *record : records) {
    if (record != nullptr && ui.nameMatches(record->get_name(), query)) {
      ++totalMatches;
    }
  }

  if (totalMatches == 0) {
    std::cout << "No records matched \"" << query << "\".\n";
    return;
  }

  std::cout << "\n"
            << CYAN << totalMatches << " record(s) found for \"" << query
            << "\".\n"
            << RESET;

  int current = 0;
  for (const User *record : records) {
    if (record == nullptr)
      continue;
    if (ui.nameMatches(record->get_name(), query)) {
      ++current;
      std::cout << "\n";
      ui.displayBMIResult(*record, current, totalMatches);
    }
  }
}

void App::deleteRecord() {
  const std::vector<const User *> records = file_manager.read_all();

  if (records.empty()) {
    std::cout << "No records to delete.\n";
    return;
  }

  ui.displayHeader("ALL RECORDS");
  ui.displayRecordList(records);

  int selection = 0;
  getInput("Enter record number to delete (" +
               std::to_string(UI::LIST_DISPLAY_OFFSET) + "-" +
               std::to_string(records.size()) + "): ",
           selection, UI::LIST_DISPLAY_OFFSET,
           static_cast<int>(records.size()));
  ui.printLine('-');

  const User *target =
      records[static_cast<size_t>(selection - UI::LIST_DISPLAY_OFFSET)];
  if (target == nullptr) {
    std::cout << RED << "Invalid record selection.\n" << RESET;
    return;
  }

  const std::string confirmPrompt =
      "Delete record for \"" + target->get_name() + "\"? (y/n): ";
  if (!ui.confirm(confirmPrompt)) {
    std::cout << "\n" << GREEN << "Deletion cancelled.\n" << RESET;
    return;
  }

  const std::string deletedName = target->get_name();
  if (file_manager.delete_by_id(target->get_id())) {
    std::cout << GREEN << "\nRecord for \"" << deletedName << "\" deleted.\n" << RESET;
  }
}
