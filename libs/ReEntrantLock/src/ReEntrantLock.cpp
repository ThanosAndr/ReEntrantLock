#include "ReEntrantLock.hpp"

void ReentrantLock::lock() {
		std::unique_lock<std::mutex> lk(m_mutex);
		if(m_count > 0) {
			if(m_owner_id == std::this_thread::get_id()) {
				++m_count;
				lk.unlock();
				return;
			} else {
				while (m_count > 0) {
				m_cv.wait(lk);
				}
			}	
		}
		m_owner_id = std::this_thread::get_id();
		m_count = 1;
		lk.unlock();
}

void ReentrantLock::unlock() {
		std::unique_lock<std::mutex> lk(m_mutex);
		if (m_count == 0) {
			lk.unlock();
			throw std::logic_error("Already Unlocked");
		}
		if(m_owner_id !=  std::this_thread::get_id()) {
			lk.unlock();
			throw std::logic_error("Unlock called by non-owner thread");
		}
		if (--m_count == 0) {
			lk.unlock();
			m_cv.notify_all();
		} 
	}