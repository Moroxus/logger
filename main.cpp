#include <logger.h>
#include <thread>

using namespace moroxus;

void function(int threadid, bool file) {
    if (file) {
        Logger::enableFile();
    }
    for (int var = 0; var < 10; ++var) {
        LOG(LogLevel::INFO) << "Thread number " << threadid << "; value: " << threadid*var;
    }
}

int main()
{
    LOG(LogLevel::INFO) << "serus z remastered loggera :D ";
    LOG(LogLevel::ERROR) << "ahoj" << " kámo " << "ako žiješ? :D :D";
    std::thread t1(function,1, false);
    std::thread t2(function,2, false);
    t1.join();
    t2.join();
    return 0;
}
