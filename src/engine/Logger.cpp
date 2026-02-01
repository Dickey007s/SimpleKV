#include "Logger.h"
#include <cstdio>
#include <fcntl.h>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace SimpleKV {

Logger::Logger(const std::string &filepath) {
  fd = ::open(filepath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);

  if (fd < 0) {
    throw std::runtime_error("Failed to open: " + filepath);
  }
}

Logger::~Logger() {
  if (fd >= 0) {
    ::close(fd);
  }
}

void Logger::Append(const std::string &key, const std::string &value) {
  std::string entry = key + ":" + value + "\n";

  ssize_t bytes_written = ::write(fd, entry.c_str(), entry.size());

  if (bytes_written < 0) {
    perror("write error\n");
  }
}

} // namespace SimpleKV