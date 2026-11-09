#pragma once

namespace concurrency {

// Minimal lock interface. Satisfies the standard BasicLockable requirements,
// so any implementation can be used with std::lock_guard<Lock>.
class Lock {
public:
	Lock() = default;
	virtual ~Lock() = default;

	Lock(const Lock&) = delete;
	Lock& operator=(const Lock&) = delete;

	virtual void lock() = 0;
	virtual void unlock() = 0;
};

} // namespace concurrency
