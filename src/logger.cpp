#include "logger.h"
#include <ctime>
#include <iomanip>

Logger::Logger()
    : running_(false)
    , log_level_(LogLevel::DEBUG)
    , log_path_("")
{}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::init(std::string logPath, LogLevel logLevel)
{
    running_ = true;
    log_level_ = logLevel;
    log_path_ = logPath;

    log_file_.open(logPath, std::ios::app);
    if (!log_file_.is_open())
    {
        throw std::runtime_error("Failed to open log file: " + logPath);
    }


    work_thread_ = std::thread(&Logger::workThread, this);
}


void Logger::setLogLevel(LogLevel logLevel)
{
    log_level_ = logLevel;
}


void Logger::debug(const std::string& message)
{
    log(message, LogLevel::DEBUG);
}

void Logger::info(const std::string& message)
{
    log(message, LogLevel::INFO);
}
void Logger::warning(const std::string& message)
{
    log(message, LogLevel::WARNING);
}
void Logger::error(const std::string& message)
{
    log(message, LogLevel::ERROR);
}

void Logger::log(const std::string& message, LogLevel logLevel)
{
    if (logLevel < log_level_)
    {
        return;
    }

    std::lock_guard<std::mutex> locker(mutex_);
    std::ostringstream oss;
    oss << getCurrentTime() << " [" << logLevelToString(logLevel) << "] " <<  message;
    log_queue_.push(oss.str());
    condition_.notify_all();

}

std::string Logger::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel logLevel)
{
    switch (logLevel)
    {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void Logger::workThread()
{
    while (running_)
    {
        std::unique_lock<std::mutex> locker(mutex_);
        condition_.wait(locker, [this] () 
        {
            return !log_queue_.empty() || !running_;
        });

        if (!running_)
        {
            break;
        }
        

        if (!log_queue_.empty())
        {
            std::string message = log_queue_.front();
            log_queue_.pop();
            locker.unlock();

            log_file_ << message << std::endl;
            log_file_.flush();
            locker.lock();
        }
    }
}

Logger::~Logger()
{
    running_ = false;
    condition_.notify_all();

    if (work_thread_.joinable())
    {
        work_thread_.join();
    }

    if (log_file_.is_open())
    {
        log_file_.close();
    }
}