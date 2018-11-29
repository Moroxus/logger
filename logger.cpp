#include "logger.h"

using namespace std;

ofstream moroxus::Logger::logFile;
mutex moroxus::Logger::mutex;
moroxus::LogLevel moroxus::Logger::logLevel = moroxus::LogLevel::INFO;

thread_local stringstream moroxus::Logger::sstream;

bool moroxus::Logger::consoleEnabled    = true;
bool moroxus::Logger::fileEnabled       = false;

bool moroxus::Logger::streamIsEmpty() {
    auto actual = sstream.tellg();
    sstream.seekg(0, std::ios_base::end);
    auto end = sstream.tellg();
    sstream.seekg(actual);
    return actual == end;
}

moroxus::Logger::Logger(LogLevel logLevel, const char * file, int line):currentLevel(logLevel) {
    if (currentLevel <= Logger::logLevel) {
        sstream << logLevel <<" FILE: " << file << " LINE: " << line << " ";
    }
}

void moroxus::Logger::setLogLevel(LogLevel logLevel) {
    lock_guard<std::mutex> lock(mutex);
    Logger::logLevel = logLevel;
}

moroxus::LogLevel moroxus::Logger::getLogLevel() {
    lock_guard<std::mutex> lock(mutex);
    return Logger::logLevel;
}

void moroxus::Logger::enableFile(std::string file) {
    lock_guard<std::mutex> locks(mutex);
    logFile.open(file, ofstream::out | ofstream::app);
    if(!logFile) {
        fileEnabled = false;
    } else {
        fileEnabled = true;
    }
}

void moroxus::Logger::disableFile() {
    lock_guard<std::mutex> locks(mutex);
    logFile.close();
    fileEnabled = false;
}

void moroxus::Logger::enableConsole() {
    lock_guard<std::mutex> locks(mutex);
    consoleEnabled = true;
}

void moroxus::Logger::disableConsole() {
    lock_guard<std::mutex> locks(mutex);
    consoleEnabled = false;
}

moroxus::Logger::~Logger() {
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

moroxus::Logger moroxus::log(LogLevel logLevel, const char * file, int line) {
    return Logger(logLevel, file, line);
}
