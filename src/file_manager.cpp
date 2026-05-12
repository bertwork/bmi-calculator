#include "file_manager.h"

namespace fs = std::filesystem;

FileManager::FileManager(const std::string &folder) {
  db_folder = folder;
  db_file_path = folder + "/records.csv";
  read_from_file();
}

void FileManager::init_database() {
  if (!fs::exists(db_folder)) {
    fs::create_directories(db_folder);
  }

  if (!fs::exists(db_file_path)) {
    std::ofstream file(db_file_path);
    if (file.is_open()) {
      file << "id,name,gender,age,height,weight,bmi,category,advice,risk\n";
      file.close();
      std::cout << "Database initialized at " << db_file_path << "\n";
    } else {
      std::cerr << "Failed to create database file!\n";
    }
  } else {
    std::cout << "Database ready at " << db_file_path << "\n";
  }
}

void FileManager::read_from_file() {
  if (!fs::exists(db_file_path)) {
    return;
  }

  std::ifstream file(db_file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open database file for reading!\n";
    return;
  }

  std::string line;
  bool firstLine = true;

  while (std::getline(file, line)) {
    if (firstLine) {
      firstLine = false;
      continue;
    }
    if (line.empty())
      continue;
  }

  file.close();
}