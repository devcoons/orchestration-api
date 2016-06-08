#include "Semaphore.h"

void Semaphore::wait()
{
	std::unique_lock<std::mutex> lk(m_);
	cv_.wait(lk, [=]{ return 0 < count_; });
	--count_;
}

bool Semaphore::try_wait()
{
	std::lock_guard<std::mutex> lk(m_);
	if (0 < count_) {
		--count_;
		return true;
	}
	else {
		return false;
	}
}

void Semaphore::signal()
{
	std::lock_guard<std::mutex> lk(m_);
	if (count_ < count_max) {
		++count_;
		cv_.notify_one();
	}
}

void Semaphore::lock() 
{ 
	wait(); 
}

bool Semaphore::try_lock() 
{ 
	return try_wait(); 
}

void Semaphore::unlock() 
{ 
	signal(); 
}