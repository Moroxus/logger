#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

#define DEBUG() debug(__FILE__, __LINE__)

namespace moroxus {

    enum class LogLevel {ERROR, WARNING, INFO, DEBUG};

    class Logger
    {
    private:

        bool consoleEnabled;
        bool fileEnabled;
        LogLevel logLevel;
        std::stringstream sstream;
        static std::ofstream logFile;
        static std::mutex mutex;

        bool isLastNewline();

        bool streamIsEmpty();

    public:

        Logger(LogLevel loglevel = LogLevel::INFO, bool consoleEnable = true, bool fileEnable = false);
        Logger &operator()(LogLevel);

        template <typename T>
        Logger &operator<<(T const value) {
            sstream << value;

            if (isLastNewline()) {
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    if (consoleEnabled) {
                        std::clog << sstream.str();
                        std::clog.flush();
                    }
                    if (fileEnabled) {
                        logFile << sstream.str();
                    }
                }
                sstream.str("");
                sstream.clear();
            }

            return *this;
        }
        void enableFile(std::string fileEnabled = "log");

        void disableFile();

        void enableConsole();

        void disableConsole();

        Logger &debug(const char *, int);

        ~Logger();
    };

}

#endif // LOGGER_H
