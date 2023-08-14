#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

#include "Lock.hpp"

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
	}

private:
	Lock * lock;
	std::string shared;

};