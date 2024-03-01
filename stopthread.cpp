#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
    std::stop_source stopSrc;

    std::stop_callback cb(stopSrc.get_token(),
        []() { std::cout << "Callback called!\n"; });

    std::jthread jt0([](std::stop_token tk) {
        for (int i = 0; i < 1000000000; i++) {
            std::cout << "Printing value: " << i << '\n';

            if (tk.stop_requested()) {
                std::cout << "Thread stopped!\n";
                return;
            }
        }
        });

    std::this_thread::sleep_for((2s));

    stopSrc.request_stop();
    std::cout << "Request to stop thread!\n";

    return 0;
}