#include <thread>

#include "ReEntrantLock.hpp"
#include "SharedClass.hpp"

int main()
{
	concurrency::ReEntrantLock lock;
	concurrency::SharedClass sharedInstance(lock);

	std::thread t1(&concurrency::SharedClass::functionA, &sharedInstance);
	std::thread t2(&concurrency::SharedClass::functionB, &sharedInstance);

	t1.join();
	t2.join();
}
