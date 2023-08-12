#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

#include "Lock.hpp"

class ReentrantLock : public Lock {
	friend class ReentrantLockTest_MultipleThreads_Test;
	friend class ReentrantLockTest_MultipleThreadsDiferentFunctions_Test;
	friend class ReentrantLockTest_UnlockCalledByNonOwnerThread_Test;
	friend class ReentrantLockTest_MultipleLockUnlockSingleThread_Test;
public:
	ReentrantLock()
	:	m_mutex(),
		m_cv(),
		m_owner_id(),
		m_count()
	{}

	virtual ~ReentrantLock() {}

	void lock() override;

	void unlock() override; 

private:
	std::mutex m_mutex;
	std::thread::id m_owner_id;
	std::condition_variable m_cv;
	std::atomic<int> m_count {0};
};