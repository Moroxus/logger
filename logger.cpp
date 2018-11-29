#include "logger.h"

using namespace std;

namespace moroxus {
    ofstream    Logger::logFile;
    mutex       Logger::mutex;
    LogLevel    Logger::logLevel = LogLevel::INFO;

    thread_local stringstream Logger::sstream;

    bool Logger::consoleEnabled    = true;
    bool Logger::fileEnabled       = false;

    bool Logger::streamIsEmpty() {
        auto actual = sstream.tellg();
        sstream.seekg(0, std::ios_base::end);
        auto end = sstream.tellg();
        sstream.seekg(actual);
        return actual == end;
    }

    Logger::Logger(LogLevel logLevel, const char * file, int line):currentLevel(logLevel) {
        if (currentLevel <= Logger::logLevel) {
            sstream << logLevel <<" FILE: " << file << " LINE: " << line << " ";
        }
    }

    void Logger::setLogLevel(LogLevel logLevel) {
        lock_guard<std::mutex> lock(mutex);
        Logger::logLevel = logLevel;
    }

    LogLevel Logger::getLogLevel() {
        lock_guard<std::mutex> lock(mutex);
        return Logger::logLevel;
    }

    void Logger::enableFile(string file) {
        lock_guard<std::mutex> locks(mutex);
        logFile.open(file, ofstream::out | ofstream::app);
        if(!logFile) {
            fileEnabled = false;
        } else {
            fileEnabled = true;
        }
    }

    void Logger::disableFile() {
        lock_guard<std::mutex> locks(mutex);
        logFile.close();
        fileEnabled = false;
    }

    void Logger::enableConsole() {
        lock_guard<std::mutex> locks(mutex);
        consoleEnabled = true;
    }

    void Logger::disableConsole() {
        lock_guard<std::mutex> locks(mutex);
        consoleEnabled = false;
    }

    Logger::~Logger() {
        if (!streamIsEmpty()) {
            sstream << "\n";
            lock_guard<std::mutex> lock(mutex);
            if (consoleEnabled) {
                clog << sstream.str();
            }
            if (fileEnabled) {
                logFile << sstream.str();
            }
            sstream.str("");
            sstream.clear();
        }
    }
}
