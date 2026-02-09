// src/server/TcpServer.cpp
#include "TcpServer.h"
#include <arpa/inet.h> // inet_ntoa
#include <cstring>     // memset
#include <iostream>
#include <stdexcept>    // std::runtime_error
#include <sys/socket.h> // socket, bind, listen, accept
#include <unistd.h>     // close

namespace SimpleKV {

TcpServer::TcpServer(int port) : port_(port), listen_fd_(-1) {
  // 在构造函数中完成初始化，如果失败直接抛出异常，防止半成品的 Server 对象存在
  init_socket();
}

TcpServer::~TcpServer() {
  // RAII: 对象销毁时，自动关闭文件描述符
  if (listen_fd_ != -1) {
    ::close(listen_fd_); // 使用全局命名空间的 close 系统调用
    std::cout << "[Server] Listener closed." << std::endl;
  }
}

void TcpServer::init_socket() {
  // 1. 创建 Socket (买个电话机)
  // AF_INET: IPv4 协议
  // SOCK_STREAM: TCP 协议 (面向连接的流式传输)
  // 0: 自动选择默认协议
  listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd_ < 0) {
    throw std::runtime_error("Failed to create socket");
  }

  // [关键点] 设置端口复用 (SO_REUSEADDR)
  // 这是一个开发阶段非常重要的选项。
  // 如果你不加这行，当你强制停止服务器后立刻重启，系统会报错 "Address already in use"，
  // 因为 TCP 连接处于 TIME_WAIT 状态，端口会被占用几分钟。这行代码允许我们立即重启复用端口。
  int opt = 1;
  setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // 2. 绑定地址和端口 (插电话线)
  struct sockaddr_in server_addr;
  std::memset(&server_addr, 0, sizeof(server_addr)); //以此清空结构体

  server_addr.sin_family = AF_INET;         // 使用 IPv4
  server_addr.sin_addr.s_addr = INADDR_ANY; // 绑定到本机所有网卡 (0.0.0.0)
  server_addr.sin_port = htons(port_);      // 端口号 (Host TO Network Short)
  // 注意：网络传输必须使用“大端序”，而你的 x86 CPU 是“小端序”，htons 负责转换。

  if (::bind(listen_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    throw std::runtime_error("Failed to bind port");
  }

  // 3. 开始监听 (打开铃声)
  // SOMAXCONN: 操作系统允许的“等待连接队列”的最大长度 (通常是 128 或更大)
  if (::listen(listen_fd_, SOMAXCONN) < 0) {
    throw std::runtime_error("Failed to listen");
  }

  std::cout << "[Server] Listening on port " << port_ << "..." << std::endl;
}

void TcpServer::Start() {
  std::cout << "[Server] SimpleKV Server started. Waiting for connections..." << std::endl;

  while (true) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 4. 接受连接 (接电话)
    // 这是一个“阻塞”调用。如果没有客户端连接，程序会停在这里不动，不消耗 CPU。
    // 一旦有连接，它返回一个新的 fd (client_fd)，专门用于和这个特定的客户端说话。
    // 原来的 listen_fd_ 继续监听别人的电话。
    int client_fd = ::accept(listen_fd_, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0) {
      std::cerr << "[Server] Accept failed" << std::endl;
      continue; // 忽略错误，继续等待下一个
    }

    // 打印客户端信息
    std::cout << "[Server] New connection from: " << inet_ntoa(client_addr.sin_addr) << ":"
              << ntohs(client_addr.sin_port) << " (fd: " << client_fd << ")" << std::endl;

    // --- 临时测试代码 ---
    // 目前我们还没有实现协议解析，只是简单地发个欢迎消息，然后关闭连接。
    const char *msg = "Hello from SimpleKV!\n";
    ::write(client_fd, msg, strlen(msg));
    ::close(client_fd);
    // ------------------
  }
}

} // namespace SimpleKV