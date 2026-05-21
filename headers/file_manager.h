#pragma once

#include "user.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

class FileManager {
public:
  FileManager(const std::string &folder = "database");
  ~FileManager();

  void init_database();
  int getRecordCount() const;

  void create(User *user);
  std::vector<User *> read_all() const;
  bool delete_by_id(int id);

private:
  std::string db_folder;
  std::string db_file_path;
  std::vector<User *> records;

  void read_from_file();
  void write_to_file();
  int get_next_id();
};
