#pragma once

#include "file_manager.h"
#include "ui.h"

#include <string>

class App {
public:
  App(const std::string &db_folder = "database");
  void run();

private:
  static constexpr int QUICK_CALC_AGE = 0;
  static constexpr const char ANONYMOUS_NAME[] = "Anonymous";
  static constexpr const char UNKNOWN_GENDER[] = "-";

  FileManager file_manager;
  UI ui;

  void handleMenuChoice(UI::MenuOption choice);
  void quickCalculate();
  void saveRecord();
  void viewRecords();
  void searchRecord();
  void deleteRecord();
};
