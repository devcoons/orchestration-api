#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
	explicit Semaphore(int init_count = count_max)	: count_(init_count) {}
	void wait();
	void lock();
	void signal();
	void unlock();
	bool try_wait();
	bool try_lock();
private:
	int count_;
	std::mutex m_;
	std::condition_variable cv_;
	static const int count_max = 1;
};

