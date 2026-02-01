#pragma once
#include <string>

namespace SimpleKV {

class Logger {
public:
  explicit Logger(const std::string &filepath);
  ~Logger();

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  void Append(const std::string &key, const std::string &value);

private:
  int fd;
};

} // namespace SimpleKV