#include "ReEntrantLock.hpp"

#include <stdexcept>

namespace concurrency {

void ReEntrantLock::lock() {
	std::unique_lock<std::mutex> lk(m_mutex);
	const auto me = std::this_thread::get_id();
	if (m_count > 0 && m_owner_id == me) {
		++m_count;
		return;
	}
	m_cv.wait(lk, [this] { return m_count == 0; });
	m_owner_id = me;
	m_count = 1;
}

bool ReEntrantLock::try_lock() {
	std::lock_guard<std::mutex> lk(m_mutex);
	const auto me = std::this_thread::get_id();
	if (m_count == 0) {
		m_owner_id = me;
		m_count = 1;
		return true;
	}
	if (m_owner_id == me) {
		++m_count;
		return true;
	}
	return false;
}

void ReEntrantLock::unlock() {
	std::lock_guard<std::mutex> lk(m_mutex);
	if (m_count == 0) {
		throw std::logic_error("Already unlocked");
	}
	if (m_owner_id != std::this_thread::get_id()) {
		throw std::logic_error("Unlock called by non-owner thread");
	}
	if (--m_count == 0) {
		m_owner_id = std::thread::id();
		m_cv.notify_one();
	}
}

} // namespace concurrency
