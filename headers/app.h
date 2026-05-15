#pragma once

#include "file_manager.h"
#include "ui.h"

#include <string>

class App {
private:
  FileManager file_manager;
  UI ui;

public:
  App(const std::string &db_folder = "database");
  void run();
};