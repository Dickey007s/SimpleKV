// src/main.cpp
#include "server/TcpServer.h"
#include <iostream>

int main() {
  try {
    // 创建服务器实例，监听 8080 端口
    SimpleKV::TcpServer server(8080);

    // 启动循环
    server.Start();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}