#pragma once

#include <netinet/in.h>
#include <string>

namespace SimpleKV {

class TcpServer {
public:
  explicit TcpServer(int port);
  ~TcpServer();

  void Start();

private:
  int port_;
  int listen_fd_;

  void init_socket();
};

} // namespace SimpleKV