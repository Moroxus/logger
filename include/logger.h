#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include "enum.h"
#include "logger_export.h"

#define LOG(x) Logger(x, __FILE__, __LINE__)

namespace moroxus {

    DECLARE_ENUM(LogLevel, NONE, ERROR, WARNING, INFO, DEBUG)
    /**
     * @brief The Logger class class for thread safe logging
     */
    class Logger
    {
    private:
        static std::ofstream                            logFile;
        static std::mutex                               mutex;
        LOGGER_EXPORT static LogLevel                   logLevel;
        static bool                                     consoleEnabled;
        static bool                                     fileEnabled;

        static thread_local std::stringstream           sstream;

        LogLevel                                        currentLevel = LogLevel::INFO;

        bool streamIsEmpty();

    private:
        LOGGER_EXPORT std::stringstream& stream();

    public:
        LOGGER_EXPORT Logger(LogLevel logLevel, const char *file, int line);

        template <typename T>
        Logger &operator<<(T const message) {
            if (currentLevel <= Logger::logLevel) {
                stream() << message;
            }
            return *this;
        }
        /**
         * @brief setLogLevel Set level of logging messages to be on the output.
         * If Loglevel is set to ERROR, lower level messages (WARNING, INFO, DEBUG)
         * will not be outputted.
         * @param logLevel
         */
        LOGGER_EXPORT static void setLogLevel(LogLevel logLevel);
        /**
         * @brief getLogLevel returns Current loglevel of messages to be on the output
         * @return moroxus::LogLevel
         */
        LOGGER_EXPORT static LogLevel getLogLevel();
        /**
         * @brief enableFile Enables logging into the file and sets name of that file.
         * Influences all threads (All threads will start logging into that file).
         * @param fileEnabled name of file for logging into
         */
        LOGGER_EXPORT static void enableFile(std::string fileEnabled = "log");
        /**
         * @brief disableFile Disables logging into file and influences all threads
         * (All threads will stop logging into file).
         */
        LOGGER_EXPORT static void disableFile();
        /**
         * @brief enableConsole Enables logging to the console and influences all threads
         * (All threads will start logging into console).
         */
        LOGGER_EXPORT static void enableConsole();
        /**
         * @brief disableConsole Disables logging to the console and influences all threads
         * (All threads will start logging into console).
         */
        LOGGER_EXPORT static void disableConsole();

        LOGGER_EXPORT ~Logger();
    };
}

#endif // LOGGER_H
