#include "ReEntrantLock.hpp"
#include "SharedClass.hpp"

int main() 
{
	std::shared_ptr<ReentrantLock> r_lock;
	r_lock = std::make_shared<ReentrantLock>();
	
	SharedClass sharedInstance(r_lock.get());

	std::thread t1(&SharedClass::functionA, &sharedInstance);
	std::thread t2(&SharedClass::functionB, &sharedInstance);

	t1.join();
	t2.join();
}