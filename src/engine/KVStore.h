#pragma once
#include "Logger.h"
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace SimpleKV {

class KVStore {
public:
  explicit KVStore(const std::string &data_path);
  ~KVStore();

  KVStore(const KVStore &) = delete;
  KVStore &operator=(const KVStore &) = delete;

  bool Put(const std::string &key, const std::string &value);
  std::optional<std::string> Get(const std::string &key);

private:
  void load_from_disk();

  std::string data_path_;
  std::unordered_map<std::string, std::string> mem_table_;
  std::unique_ptr<Logger> logger_;
  std::mutex mutex_;
};

} // namespace SimpleKV