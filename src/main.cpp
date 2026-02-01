#include "engine/Logger.h"
#include <iostream>

using namespace SimpleKV;

int main() {
  try {
    std::cout << "SimpleKV starting..." << std::endl;

    // 初始化 Logger，指向 ../data/data.log
    Logger logger("../data/data.log");

    // 写入
    logger.Append("username", "jerry");
    logger.Append("language", "cpp");
    logger.Append("system", "linux");

    std::cout << "Write success! Check ./data/data.log" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}