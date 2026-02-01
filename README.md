# SimpleKV

一个基于 C++17/20 和 Linux 系统调用的高性能极简 KV 存储引擎。

## 🌟 项目亮点
- **系统级 I/O**：不调用标准库封装，直接使用 `open`, `write` 等系统调用进行磁盘操作。
- **RAII 资源管理**：使用现代 C++ 智能指针与析构机制，确保文件描述符无泄漏。
- **WAL 机制**：实现预写日志（Logger），保证数据写入的顺序性与持久化。

## 🚀 快速开始

### 1. 环境准备
- GCC 9.0+
- CMake 3.10+
- WSL (Ubuntu 22.0+)

### 2. 编译运行
```bash
mkdir build && cd build
cmake ..
make
./simplekv
```