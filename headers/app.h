#pragma once

#include "file_manager.h"

#include <string>

class App {
private:
  FileManager file_manager;

public:
  App(const std::string &db_folder = "database");
  void run();
};