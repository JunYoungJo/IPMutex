#include "ipm/IPMutex.h"
#include <iostream>
#include <mutex>

int main(int argc, char **argv) {

    std::string key;

    if (argc >= 2) {
        key = argv[1];
    }

    ipm::IPMutex mutex{key};

    std::cout << "process(" << getpid() << ") : try to lock\n";
    {
        std::lock_guard lock{mutex};
        std::cout << "process(" << getpid() << ") : get lock\n";
        for (int i = 0; i < 10; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "process(" << getpid() << ") : Holding lock for " << i << " seconds...\n";
        }
    }

    // The owner must terminate after other users have terminated.
    if (mutex.isOwner()) {
        std::string buf;
        while (buf != "q") {
            std::cin >> buf;
            if (buf == "r") {
                {
                    std::lock_guard lock{mutex};
                    std::cout << "process(" << getpid() << ") : get lock\n";
                    for (int i = 0; i < 10; i++) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        std::cout << "process(" << getpid() << ") : Holding lock for " << i << " seconds...\n";
                    }
                }
            }
        }
    }

    return 0;
}
