#include "logger.h"

std::ofstream moroxus::Logger::logFile;
std::mutex moroxus::Logger::mutex;
moroxus::LogLevel moroxus::Logger::logLevel = moroxus::LogLevel::INFO;

thread_local std::stringstream moroxus::Logger::sstream;

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

void moroxus::Logger::enableFile(std::string file) {
    logFile.open(file, std::ofstream::out | std::ofstream::app);
    if(!logFile) {
        fileEnabled = false;
    } else {
        fileEnabled = true;
    }
}

void moroxus::Logger::disableFile() {
    logFile.close();
    fileEnabled = false;
}

void moroxus::Logger::enableConsole() {
    consoleEnabled = true;
}

void moroxus::Logger::disableConsole() {
    consoleEnabled = false;
}

moroxus::Logger::~Logger() {
    if (!streamIsEmpty()) {
        sstream << "\n";
        std::lock_guard<std::mutex> lock(mutex);
        if (consoleEnabled) {
            std::clog << sstream.str();
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
