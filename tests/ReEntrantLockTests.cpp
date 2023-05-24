#include <gtest/gtest.h>
#include <vector>

#include "ReEntrantLock.hpp"


TEST(ReentrantLockTest, MultipleThreads) {
	ReentrantLock lock;
	std::atomic<int> counter {0};

	auto increment = [&lock, &counter]() {
		for (int i = 0; i < 5000; ++i) {
			lock.lock();
			++counter;
			lock.unlock();
		}
	};

	std::thread t1(increment);
	std::thread t2(increment);

	t1.join();
	t2.join();

	EXPECT_EQ(lock.m_count, 0);
	EXPECT_EQ(counter, 10000);
}

TEST(ReentrantLockTest, MultipleThreadsDiferentFunctions) {
	ReentrantLock lock;
	std::atomic<int> counter {0};

	auto increment = [&lock, &counter]() {
		for (int i = 0; i < 50000; ++i) {
			lock.lock();
			++counter;
			lock.unlock();
		}
	};

	auto decrement = [&lock, &counter]() {
		for (int i = 0; i < 50000; ++i) {
			lock.lock();
			--counter;
			lock.unlock();
		}
	};

	std::thread t1(increment);
	std::thread t2(decrement);

	t1.join();
	t2.join();

	EXPECT_EQ(lock.m_count, 0);
	EXPECT_EQ(counter, 0);
}

TEST(ReentrantLockTest, MultipleLockUnlockSingleThread) {
	ReentrantLock lock;
	std::atomic<int> counter {0};

	auto increment = [&lock, &counter]() {
		for (int i = 0; i < 1000; ++i) {
			lock.lock();
			for (int j = 0; j < 1000; ++j) {
				lock.lock();
				++counter;
				lock.unlock();
			}
			lock.unlock();
		}
	};

	std::thread t1(increment);

	t1.join();

	EXPECT_EQ(lock.m_count, 0);
	EXPECT_EQ(counter, 1000000);
}

TEST(ReentrantLockTest, Concurrent) {
	ReentrantLock lock;
	std::atomic<int> counter{0};
	std::vector<std::thread> threads;

	for (int i = 0; i < 100; i++) {
		threads.emplace_back([&lock, &counter]() {
			for (int j = 0; j < 100; j++) {
				lock.lock();
				counter++;
				lock.unlock();
			}
		});
	}
	for (auto& thread : threads) {
		thread.join();
	}

	EXPECT_EQ(counter.load(), 10000);
}

TEST(ReentrantLockTest, SingleThread) {
	ReentrantLock lock;

	lock.lock();
	
	EXPECT_NO_THROW(lock.unlock());
}

TEST(ReentrantLockTest, Recursive) {
	ReentrantLock lock;

	lock.lock();
	
	EXPECT_NO_THROW(lock.lock());
	EXPECT_NO_THROW(lock.unlock());
	EXPECT_NO_THROW(lock.unlock());
}

TEST(ReentrantLockTest, TryToUnlockAThreadThatIsNotLocked) {
	ReentrantLock lock;

	EXPECT_THROW(lock.unlock(), std::logic_error);
}

TEST(ReentrantLockTest, UnlockTwice) {
	ReentrantLock lock;

	lock.lock();
	
	EXPECT_NO_THROW(lock.unlock());
	EXPECT_THROW(lock.unlock(), std::logic_error);
}