#include "logger.h"
#include <thread>
#include <vector>
#include <chrono>

void logTest(int threadId) {
    auto& logger = Logger::getInstance();
    
    for (int i = 0; i < 10; ++i) {
        logger.debug("Thread " + std::to_string(threadId) + " debug message " + std::to_string(i));
        logger.info("Thread " + std::to_string(threadId) + " info message " + std::to_string(i));
        logger.warning("Thread " + std::to_string(threadId) + " warning message " + std::to_string(i));
        logger.error("Thread " + std::to_string(threadId) + " error message " + std::to_string(i));
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    try {
        auto& logger = Logger::getInstance();
        logger.init("test.log", LogLevel::DEBUG);
        
        std::vector<std::thread> threads;
        const int numThreads = 4;
        
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(logTest, i);
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
} 