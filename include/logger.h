#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include "enum.h"

#define LOG(x) log(x, __FILE__, __LINE__)

namespace moroxus {

    DECLARE_ENUM(LogLevel, NONE, ERROR, WARNING, INFO, DEBUG)

    class Logger
    {
    private:
        static std::ofstream                            logFile;
        static std::mutex                               mutex;
        static LogLevel                                 logLevel;
        static bool                                     consoleEnabled;
        static bool                                     fileEnabled;

        static thread_local std::stringstream           sstream;

        LogLevel                                        currentLevel = LogLevel::INFO;

        bool streamIsEmpty();

    public:
        Logger(LogLevel logLevel, const char *file, int line);

        template <typename T>
        Logger &operator<<(T const message) {
            if (currentLevel <= Logger::logLevel) {
                sstream << message;
            }
            return *this;
        }

        static void enableFile(std::string fileEnabled = "log");

        static void disableFile();

        static void enableConsole();

        static void disableConsole();

        ~Logger();
    };

    Logger log(LogLevel logLevel, const char *file, int line);

}

#endif // LOGGER_H
