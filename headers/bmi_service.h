#pragma once

#include "file_manager.h"
#include "input_utility.h"
#include "ui.h"

class BMIService {
private:
  FileManager &file_manager;
  UI &ui;

public:
  BMIService(FileManager &f_manager, UI &ui);

  void quickCalculate() const;
  void saveRecord() const;
  void viewRecords() const;
  void searchRecord() const;
  void deleteRecord() const;
};