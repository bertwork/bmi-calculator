#include "file_manager.h"

namespace fs = std::filesystem;

FileManager::FileManager(const std::string &folder)
    : db_folder(folder), db_file_path(folder + "/records.psv") {
  read_from_file();
}

void FileManager::init_database() {
  if (!fs::exists(db_folder)) {
    fs::create_directories(db_folder);
  }

  if (!fs::exists(db_file_path)) {
    std::ofstream file(db_file_path);
    if (file.is_open()) {
      file << "id|name|gender|age|height|weight|bmi|category|advice|risk\n";
      std::cout << GREEN << "Database initialized at " << db_file_path << RESET
                << "\n\n";
    } else {
      std::cerr << "Failed to create database file!\n";
    }
  } else {
    std::cout << GREEN << "Database ready at " << db_file_path << RESET << "\n\n";
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

    records.push_back(std::make_unique<User>(User::from_psv(line)));
  }
}

void FileManager::write_to_file() {
  std::ofstream file(db_file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open database file for writing!\n";
    return;
  }

  file << "id|name|gender|age|height|weight|bmi|category|advice|risk\n";
  for (const auto &user : records) {
    file << user->to_psv() << "\n";
  }
}

int FileManager::get_next_id() const {
  int maxId = 0;
  for (const auto &user : records) {
    if (user->get_id() > maxId) {
      maxId = user->get_id();
    }
  }
  return maxId + 1;
}

void FileManager::create(const User &user) {
  int newId = get_next_id();
  auto stored = std::make_unique<User>(user);

  stored->set_id(newId);
  records.push_back(std::move(stored));

  write_to_file();
  backup();
  std::cout << GREEN << "\nRecord saved! (ID: " << newId << ")\n" << RESET;
}

void FileManager::backup() {
  if (!fs::exists(db_file_path)) {
    return;
  }

  const std::string backup_folder = db_folder + "/backup";
  if (!fs::exists(backup_folder)) {
    fs::create_directories(backup_folder);
  }

  const auto now = std::chrono::system_clock::now();
  const std::time_t time = std::chrono::system_clock::to_time_t(now);
  std::tm localTime{};
#ifdef _WIN32
  localtime_s(&localTime, &time);
#else
  localtime_r(&time, &localTime);
#endif

  std::ostringstream timestamp;
  timestamp << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S");
  const std::string backup_path =
      backup_folder + "/records_" + timestamp.str() + ".psv";

  try {
    fs::copy_file(db_file_path, backup_path,
                  fs::copy_options::overwrite_existing);
  } catch (const fs::filesystem_error &) {
    return;
  }

  std::vector<fs::directory_entry> backupFiles;
  for (const auto &entry : fs::directory_iterator(backup_folder)) {
    if (entry.is_regular_file() && entry.path().extension() == ".psv") {
      backupFiles.push_back(entry);
    }
  }

  if (backupFiles.size() <= 3) {
    return;
  }

  std::sort(backupFiles.begin(), backupFiles.end(),
            [](const fs::directory_entry &a, const fs::directory_entry &b) {
              return fs::last_write_time(a) < fs::last_write_time(b);
            });

  while (backupFiles.size() > 3) {
    fs::remove(backupFiles.front().path());
    backupFiles.erase(backupFiles.begin());
  }
}

bool FileManager::delete_by_id(int id) {
  for (size_t i = 0; i < records.size(); ++i) {
    if (records[i]->get_id() == id) {
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

std::vector<const User *> FileManager::read_all() const {
  std::vector<const User *> ptrs;
  ptrs.reserve(records.size());

  for (const auto &u : records) {
    ptrs.push_back(u.get());
  }

  return ptrs;
}