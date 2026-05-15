#pragma once

#include "user.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class FileManager {
private:
  std::string db_folder;
  std::string db_file_path;
  std::vector<User *> records;

  void read_from_file();

public:
  FileManager(const std::string &folder = "database");
  ~FileManager();

  void init_database();
  int getRecordCount() const;
};