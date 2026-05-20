#include "bmi_service.h"

BMIService::BMIService(FileManager &f_manager, UI &ui)
    : file_manager(f_manager), ui(ui) {}

void BMIService::quickCalculate() const {
  ui.printLine('-');
  std::cout << "QUICK BMI CALCULATION SELECTED\n";
}

void BMIService::saveRecord() const {
  ui.printLine('-');
  std::cout << "SAVE BMI RECORD SELECTED\n";
}

void BMIService::viewRecords() const {
  ui.printLine('-');
  std::cout << "VIEW ALL RECORDS SELECTED\n";
}
void BMIService::searchRecord() const {
  ui.printLine('-');
  std::cout << "SEARCH RECORD SELECTED\n";
}
void BMIService::deleteRecord() const {
  ui.printLine('-');
  std::cout << "DELETE RECORD SELECTED\n";
}