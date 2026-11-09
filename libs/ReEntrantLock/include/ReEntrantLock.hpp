#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

#include "Lock.hpp"

namespace concurrency {

// A mutex that the owning thread may lock multiple times. It is released
// once unlock() has been called as many times as lock().
// Satisfies the standard Lockable requirements (lock, unlock, try_lock).
class ReEntrantLock : public Lock {
public:
	ReEntrantLock() = default;
	~ReEntrantLock() override = default;

	void lock() override;

	void unlock() override;

	// Acquires the lock without blocking. Returns true on success, including
	// when the calling thread already owns it.
	bool try_lock();

	// Snapshot of the current recursion depth (0 when unlocked).
	int get_count() const {
		std::lock_guard<std::mutex> lk(m_mutex);
		return m_count;
	}

private:
	mutable std::mutex m_mutex;
	std::condition_variable m_cv;
	std::thread::id m_owner_id;
	int m_count {0};
};

} // namespace concurrency
