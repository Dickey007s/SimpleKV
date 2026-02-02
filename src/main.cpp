#include "engine/KVStore.h"
#include <iostream>
#include <string>

using namespace SimpleKV;

void print_help() {
  std::cout << "Commands:\n"
            << "  SET <key> <value>\n"
            << "  GET <key>\n"
            << "  EXIT\n"
            << "> ";
}

int main() {
  std::cout << "=== SimpleKV Database (v0.1) ===" << std::endl;

  // 初始化引擎，自动触发 crash recovery
  KVStore kv_store("./data/data.log");

  std::string cmd;
  while (true) {
    print_help();
    std::cin >> cmd;

    if (cmd == "EXIT")
      break;

    if (cmd == "SET") {
      std::string key, value;
      std::cin >> key >> value;
      if (kv_store.Put(key, value)) {
        std::cout << "OK" << std::endl;
      } else {
        std::cerr << "Error: Write failed" << std::endl;
      }
    } else if (cmd == "GET") {
      std::string key;
      std::cin >> key;
      auto result = kv_store.Get(key);
      if (result) {
        std::cout << "\"" << *result << "\"" << std::endl;
      } else {
        std::cout << "(nil)" << std::endl;
      }
    } else {
      std::cout << "Unknown command" << std::endl;
      // 清理输入流，防止死循环
      std::string dummy;
      std::getline(std::cin, dummy);
    }
  }
  return 0;
}