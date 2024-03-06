#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore>

std::binary_semaphore producer{ 1 };
std::binary_semaphore consumer{ 0 };

void produce() {	 //produce.aqu consum.rel, cons.aq prod.rele
	for (;;) {
		producer.acquire();
		std::cout << "Produced " << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		consumer.release();
	}
};

void consume() {
	for (;;) {
		consumer.acquire();
		std::cout << consumer.max << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		producer.release();
	}
};

int main() {
	std::jthread t1(produce);
	std::jthread t2(consume);
}