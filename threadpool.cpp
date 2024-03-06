#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <memory>

class ThreadPool {
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::function<void()> task;
    bool stop;

public:

    ThreadPool(size_t num_of_threads = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < num_of_threads; i++)
        {
            workers.emplace_back([this] {
                while (true)
                {
                    std::unique_lock<std::mutex> lck(queue_mutex);
                    cv.wait(lck, [this] {return stop || !tasks.empty(); });
                    if (stop && tasks.empty())
                    {
                        return;
                    }
                    task = std::move(tasks.front());
                    tasks.pop();
                    task();
                }
                });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lck(queue_mutex);
            stop = true;
        }
        cv.notify_all();

        for (auto&& i : workers)
        {
            i.join();
        };
    }

    template<class T>
    void enqueue(T&& task) {
        {
            std::unique_lock<std::mutex> lck(queue_mutex);
            tasks.emplace(std::forward<T>(task));
        }
        cv.notify_one();
    }
};

int main() {

    auto T = std::make_unique<ThreadPool>(2);

    for (int i = 0; i < 8; i++)
    {
        T->enqueue([i] {
            std::cout << "Hello from thread " << i << std::endl;
            });
    };

    std::this_thread::sleep_for(std::chrono::seconds(2));
};