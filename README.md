# 高性能多线程日志系统

这是一个用C++实现的高性能多线程日志系统，具有以下特点：

- 异步日志写入
- 多线程安全
- 高性能缓冲区设计
- 日志级别控制

## 功能特性

- 支持四种日志级别：DEBUG、INFO、WARNING、ERROR
- 异步写入日志，不影响主程序性能
- 线程安全，支持多线程并发写入
- 单例模式实现，全局唯一实例

## 编译方法

```bash
mkdir build
cd build
cmake ..
make
```

## 使用方法

```cpp
#include "logger.h"

int main() {
    // 初始化日志系统
    auto& logger = Logger::getInstance();
    logger.init("app.log", LogLevel::INFO);  // 设置日志级别为INFO
    
    // 记录日志
    logger.debug("这是一条调试信息");
    logger.info("这是一条普通信息");
    logger.warning("这是一条警告信息");
    logger.error("这是一条错误信息");
    
    // 动态修改日志级别
    logger.setLogLevel(LogLevel::WARNING);
    
    return 0;
}
```

## 日志格式

日志格式如下：
```
YYYY-MM-DD HH:MM:SS.mmm [LEVEL] message
```

例如：
```
2024-03-21 14:30:45.123 [INFO] 这是一条普通信息
```

## 性能优化

- 使用异步写入机制，避免阻塞主线程
- 使用条件变量实现高效的生产者-消费者模式
- 使用互斥锁保证线程安全
- 使用缓冲区减少磁盘I/O操作 