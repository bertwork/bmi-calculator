#include "file_manager.h"

#include <iostream>

namespace fs = std::filesystem;

FileManager::FileManager(const std::string &folder) {
  db_folder = folder;
  db_file_path = folder + "/records.csv";
  read_from_file();
}

FileManager::~FileManager() {
  for (User *user : records) {
    delete user;
  }
  records.clear();
}

void FileManager::init_database() {
  if (!fs::exists(db_folder)) {
    fs::create_directories(db_folder);
  }

  if (!fs::exists(db_file_path)) {
    std::ofstream file(db_file_path);
    if (file.is_open()) {
      file << "id|name|gender|age|height|weight|bmi|category|advice|risk\n";
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
  bool isHeader = true;

  while (std::getline(file, line)) {
    if (isHeader) {
      isHeader = false;
      continue;
    }
    if (line.empty()) {
      continue;
    }

    records.push_back(new User(User::from_csv(line)));
  }
}

void FileManager::write_to_file() {
  std::ofstream file(db_file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open database file for writing!\n";
    return;
  }

  file << "id|name|gender|age|height|weight|bmi|category|advice|risk\n";
  for (const User *user : records) {
    if (user != nullptr) {
      file << user->to_csv() << "\n";
    }
  }
}

int FileManager::get_next_id() {
  int maxId = 0;
  for (const User *user : records) {
    if (user != nullptr && user->get_id() > maxId) {
      maxId = user->get_id();
    }
  }
  return maxId + 1;
}

void FileManager::create(User *user) {
  if (user == nullptr) {
    std::cerr << "Cannot create null user!\n";
    return;
  }

  int newId = get_next_id();
  User *stored = new User(*user);
  stored->set_id(newId);
  records.push_back(stored);
  write_to_file();
  std::cout << "Record saved! (ID: " << newId << ")\n";
}

bool FileManager::delete_by_id(int id) {
  for (size_t i = 0; i < records.size(); ++i) {
    if (records[i] != nullptr && records[i]->get_id() == id) {
      delete records[i];
      records.erase(records.begin() + static_cast<std::ptrdiff_t>(i));
      write_to_file();
      return true;
    }
  }

  std::cerr << "Record with ID " << id << " not found!\n";
  return false;
}

int FileManager::getRecordCount() const {
  return static_cast<int>(records.size());
}

std::vector<User *> FileManager::read_all() const { return records; }