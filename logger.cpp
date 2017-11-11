#include "logger.h"

std::ofstream moroxus::Logger::logFile;
std::mutex moroxus::Logger::mutex;

bool moroxus::Logger::isLastNewline() {
    int actual = sstream.tellg();
    sstream.seekg(-1, std::ios_base::end);
    char c = sstream.peek();
    sstream.seekg(actual);

    return c == '\n';
}

bool moroxus::Logger::streamIsEmpty() {
    int actual = sstream.tellg();
    sstream.seekg(0, std::ios_base::end);
    int end = sstream.tellg();
    sstream.seekg(actual);
    actual == end;
}

moroxus::Logger::Logger(moroxus::LogLevel logLevel, bool consoleEnable, bool fileEnable):logLevel(logLevel),
    consoleEnabled(consoleEnable),
    fileEnabled(fileEnable) {}

moroxus::Logger &moroxus::Logger::operator()(moroxus::LogLevel logLevel) {
    this->logLevel = logLevel;
    return *this;
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

moroxus::Logger &moroxus::Logger::debug(const char *file, int line) {
    sstream << "FILE: " << file << " LINE: " << line << "\t\t";
    return *this;
}

moroxus::Logger::~Logger() {
    if (!streamIsEmpty()) {
        std::lock_guard<std::mutex> lock(mutex);
        if (consoleEnabled) {
            std::clog << sstream.str();
        }
        if (fileEnabled) {
            logFile << sstream.str();
        }
    }
}
