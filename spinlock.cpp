#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

void critical_section(int id) {

    static std::atomic_flag lock = ATOMIC_FLAG_INIT;
    static unsigned int spin_count = 1;

    while (lock.test_and_set(std::memory_order_acquire))
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(spin_count));
        spin_count <<= 1; //incremental backoff, tho unif_dist(1,1024) performs better

        lock.clear(std::memory_order_release);

    }

    std::cout << "Thread " << id << " entered critical section." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread " << id << " exited critical section." << std::endl;

    lock.clear(std::memory_order_release);
}

int main() {

    for (int i = 0; i < 5; ++i) {
        std::jthread(critical_section, i);
    }

}