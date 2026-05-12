#include "app.h"

App::App(const std::string &db_folder) : file_manager(db_folder) {
  file_manager.init_database();
}

void App::run() { std::cout << "App is running!!!"; }
