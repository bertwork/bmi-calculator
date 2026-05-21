#pragma once

#include "file_manager.h"
#include "input_utility.h"
#include "ui.h"
#include "bmi_service.h"
#include "user.h"

#include <string>

class App {
private:
  FileManager file_manager;
  UI ui;
  BMIService bmi_service;

  void quickCalculate();
  void saveRecord();
  void viewRecords();
  void searchRecord();
  void deleteRecord();

public:
  App(const std::string &db_folder = "database");
  void run();
  void handleMenuChoice(UI::MenuOption choice);
};