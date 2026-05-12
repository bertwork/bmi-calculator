#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class FileManager {
private:
  std::string db_folder;
  std::string db_file_path;

  void read_from_file();

public:
  FileManager(const std::string &folder = "database");

  void init_database();
};