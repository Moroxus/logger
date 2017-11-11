#include <logger.h>
#include <thread>

void function(int threadid, bool file) {
    moroxus::Logger log;
    if (file) {
        log.enableFile();
    }
    for (int var = 0; var < 10; ++var) {
        log.DEBUG() << "Thread number " << threadid << "; value: " << threadid*var << "\n";
    }
}

int main()
{
    moroxus::Logger log;
    std::thread t1(function,1, true);
    std::thread t2(function,2, false);
    t1.join();
    t2.join();
    log.DEBUG() << "end";
    return 0;
}
