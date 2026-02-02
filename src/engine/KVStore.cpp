#include "KVStore.h"
#include "Logger.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>

namespace SimpleKV {

void KVStore::load_from_disk() {
  std::ifstream infile(data_path_);
  if (!infile.is_open()) {
    return;
  }

  std::string line;
  int count = 0;

  while (std::getline(infile, line)) {

    auto pos = line.find(':');

    if (pos != std::string::npos) {
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos + 1);

      mem_table_[key] = value;
      count++;
    }
  }
  std::cout << "[Engine] Recovered " << count << " records form disk.\n";
}

KVStore::KVStore(const std::string &data_path) : data_path_(data_path) {
  load_from_disk();

  logger_ = std::make_unique<Logger>(data_path);
}

KVStore::~KVStore() = default;

bool KVStore::Put(const std::string &key, const std::string &value) {
  std::lock_guard<std::mutex> lock(mutex_);

  try {
    logger_->Append(key, value);
  } catch (...) {
    std::cout << "Faild to write in disk.\n";
    return false;
  }

  mem_table_[key] = value;
  return true;
}

std::optional<std::string> KVStore::Get(const std::string &key) {
  std::lock_guard<std::mutex> lock(mutex_);

  auto it = mem_table_.find(key);
  if (it != mem_table_.end()) {
    return it->second;
  }
  return std::nullopt;
}

} // namespace SimpleKV