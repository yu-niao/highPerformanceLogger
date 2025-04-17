#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <sstream>
#include <fmt/core.h>

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    static Logger& getInstance();
    void init(std::string logPath, LogLevel logLevel = LogLevel::INFO);
    void setLogLevel(LogLevel logLevel);

    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);

    ~Logger();

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    void log(const std::string& message, LogLevel logLevel);
    void workThread();
    std::string getCurrentTime();
    std::string logLevelToString(LogLevel logLevel);

    std::string log_path_;
    LogLevel log_level_;
    bool running_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<std::string> log_queue_;
    std::thread work_thread_;
    std::ofstream log_file_;
};