#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>


class Lock {
public:
	virtual void lock() = 0;
	virtual void unlock() = 0;
	
	Lock() = default;
	
	virtual ~Lock() = default;
};
 
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
 
 
class SharedClass {
public:
	SharedClass(Lock * lock) {
		this->lock = lock;
	}

		void functionA() {
			lock->lock();

			shared = "functionA";
			std::cout << "in functionA, shared variable is now " << shared << '\n';

			lock->unlock();
		}

		void functionB() {
			lock->lock();

			shared = "functionB";
			std::cout << "in functionB, shared variable is now " << shared  << '\n';
			functionA();
			std::cout << "back in functionB, shared variable is " << shared  << '\n';

			lock->unlock();
		};

private:
	Lock * lock;
	std::string shared;

};