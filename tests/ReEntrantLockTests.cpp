#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include "ReEntrantLock.hpp"
#include "SharedClass.hpp"

using concurrency::ReEntrantLock;
using concurrency::SharedClass;

namespace {

// Tracks how many threads are inside the critical section at once.
// If the lock works, the maximum observed is exactly 1.
struct ExclusionProbe {
	std::atomic<int> inside {0};
	std::atomic<int> max_inside {0};

	void enter() {
		const int now = ++inside;
		int prev = max_inside.load();
		while (now > prev && !max_inside.compare_exchange_weak(prev, now)) {
		}
	}

	void leave() { --inside; }
};

void short_pause() {
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

} // namespace

// Counters below are deliberately NOT atomic: they only add up correctly
// if the lock really provides mutual exclusion.

TEST(ReEntrantLockTest, MultipleThreads) {
	ReEntrantLock lock;
	int counter = 0;
	ExclusionProbe probe;

	auto increment = [&]() {
		for (int i = 0; i < 5000; ++i) {
			lock.lock();
			probe.enter();
			++counter;
			probe.leave();
			lock.unlock();
		}
	};

	std::thread t1(increment);
	std::thread t2(increment);

	t1.join();
	t2.join();

	EXPECT_EQ(lock.get_count(), 0);
	EXPECT_EQ(counter, 10000);
	EXPECT_EQ(probe.max_inside, 1);
}

TEST(ReEntrantLockTest, MultipleThreadsDifferentFunctions) {
	ReEntrantLock lock;
	int counter = 0;
	ExclusionProbe probe;

	auto increment = [&]() {
		for (int i = 0; i < 50000; ++i) {
			lock.lock();
			probe.enter();
			++counter;
			probe.leave();
			lock.unlock();
		}
	};

	auto decrement = [&]() {
		for (int i = 0; i < 50000; ++i) {
			lock.lock();
			probe.enter();
			--counter;
			probe.leave();
			lock.unlock();
		}
	};

	std::thread t1(increment);
	std::thread t2(decrement);

	t1.join();
	t2.join();

	EXPECT_EQ(lock.get_count(), 0);
	EXPECT_EQ(counter, 0);
	EXPECT_EQ(probe.max_inside, 1);
}

TEST(ReEntrantLockTest, MultipleLockUnlockSingleThread) {
	ReEntrantLock lock;
	int counter = 0;

	auto increment = [&]() {
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

	EXPECT_EQ(lock.get_count(), 0);
	EXPECT_EQ(counter, 1000000);
}

TEST(ReEntrantLockTest, Concurrent) {
	ReEntrantLock lock;
	int counter = 0;
	ExclusionProbe probe;
	std::vector<std::thread> threads;

	for (int i = 0; i < 100; i++) {
		threads.emplace_back([&]() {
			for (int j = 0; j < 100; j++) {
				lock.lock();
				probe.enter();
				counter++;
				probe.leave();
				lock.unlock();
			}
		});
	}
	for (auto& thread : threads) {
		thread.join();
	}

	EXPECT_EQ(lock.get_count(), 0);
	EXPECT_EQ(counter, 10000);
	EXPECT_EQ(probe.max_inside, 1);
}

TEST(ReEntrantLockTest, SingleThread) {
	ReEntrantLock lock;

	lock.lock();
	EXPECT_EQ(lock.get_count(), 1);

	EXPECT_NO_THROW(lock.unlock());
	EXPECT_EQ(lock.get_count(), 0);
}

TEST(ReEntrantLockTest, Recursive) {
	ReEntrantLock lock;

	lock.lock();
	EXPECT_NO_THROW(lock.lock());
	EXPECT_EQ(lock.get_count(), 2);

	EXPECT_NO_THROW(lock.unlock());
	EXPECT_EQ(lock.get_count(), 1);
	EXPECT_NO_THROW(lock.unlock());
	EXPECT_EQ(lock.get_count(), 0);
}

TEST(ReEntrantLockTest, TryToUnlockAThreadThatIsNotLocked) {
	ReEntrantLock lock;

	EXPECT_THROW(lock.unlock(), std::logic_error);
}

TEST(ReEntrantLockTest, UnlockTwice) {
	ReEntrantLock lock;

	lock.lock();

	EXPECT_NO_THROW(lock.unlock());
	EXPECT_THROW(lock.unlock(), std::logic_error);
}

TEST(ReEntrantLockTest, UnlockCalledByNonOwnerThread) {
	ReEntrantLock lock;
	lock.lock();

	bool threw_logic_error = false;
	std::thread other([&]() {
		try {
			lock.unlock();
		} catch (const std::logic_error&) {
			threw_logic_error = true;
		}
	});
	other.join();

	EXPECT_TRUE(threw_logic_error);
	// The failed unlock must not have released the lock.
	EXPECT_EQ(lock.get_count(), 1);
	EXPECT_NO_THROW(lock.unlock());
}

TEST(ReEntrantLockTest, OtherThreadBlocksUntilFinalUnlock) {
	ReEntrantLock lock;
	std::atomic<bool> acquired {false};

	lock.lock();
	lock.lock();

	std::thread other([&]() {
		lock.lock();
		acquired = true;
		lock.unlock();
	});

	short_pause();
	EXPECT_FALSE(acquired) << "other thread got the lock while it was held twice";

	lock.unlock();
	short_pause();
	EXPECT_FALSE(acquired) << "other thread got the lock while it was still held once";

	lock.unlock();
	other.join();
	EXPECT_TRUE(acquired);
	EXPECT_EQ(lock.get_count(), 0);
}

TEST(ReEntrantLockTest, SharedClassWithTwoThreads) {
	ReEntrantLock lock;
	SharedClass shared(lock);

	// functionB re-enters the lock by calling functionA; this would
	// deadlock with a non-reentrant lock.
	std::thread t1([&]() { for (int i = 0; i < 20; ++i) shared.functionA(); });
	std::thread t2([&]() { for (int i = 0; i < 20; ++i) shared.functionB(); });

	t1.join();
	t2.join();

	EXPECT_EQ(lock.get_count(), 0);
}

TEST(ReEntrantLockTest, TryLockWhenFree) {
	ReEntrantLock lock;

	EXPECT_TRUE(lock.try_lock());
	EXPECT_EQ(lock.get_count(), 1);
	lock.unlock();
	EXPECT_EQ(lock.get_count(), 0);
}

TEST(ReEntrantLockTest, TryLockReentersForOwner) {
	ReEntrantLock lock;
	lock.lock();

	EXPECT_TRUE(lock.try_lock());
	EXPECT_EQ(lock.get_count(), 2);

	lock.unlock();
	lock.unlock();
	EXPECT_EQ(lock.get_count(), 0);
}

TEST(ReEntrantLockTest, TryLockFailsWhenHeldByOtherThread) {
	ReEntrantLock lock;
	lock.lock();

	bool other_got_it = true;
	std::thread other([&]() { other_got_it = lock.try_lock(); });
	other.join();

	EXPECT_FALSE(other_got_it);
	EXPECT_EQ(lock.get_count(), 1);
	lock.unlock();
}

TEST(ReEntrantLockTest, WorksWithStandardLockWrappers) {
	ReEntrantLock lock;
	{
		std::lock_guard<ReEntrantLock> outer(lock);
		std::unique_lock<ReEntrantLock> inner(lock, std::try_to_lock);
		EXPECT_TRUE(inner.owns_lock());
		EXPECT_EQ(lock.get_count(), 2);
	}
	EXPECT_EQ(lock.get_count(), 0);
}
