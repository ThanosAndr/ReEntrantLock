#pragma once

#include <iostream>
#include <mutex>
#include <string>

#include "Lock.hpp"

namespace concurrency {

// Demo class: functionB calls functionA while already holding the lock,
// which only works if the lock is reentrant.
class SharedClass {
public:
	explicit SharedClass(Lock& lock)
	:	m_lock(lock)
	{}

	void functionA() {
		std::lock_guard<Lock> guard(m_lock);

		m_shared = "functionA";
		std::cout << "in functionA, shared variable is now " << m_shared << '\n';
	}

	void functionB() {
		std::lock_guard<Lock> guard(m_lock);

		m_shared = "functionB";
		std::cout << "in functionB, shared variable is now " << m_shared << '\n';
		functionA();
		std::cout << "back in functionB, shared variable is " << m_shared << '\n';
	}

private:
	Lock& m_lock;
	std::string m_shared;
};

} // namespace concurrency
