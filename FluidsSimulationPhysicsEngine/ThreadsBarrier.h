#pragma once

#include <thread>
#include <atomic>
#include <mutex>

class ThreadsBarrier
{
public:
	explicit ThreadsBarrier(unsigned numThreads)
		: m_numThreads(numThreads)
		, m_counts{ 0, 0 }
		, m_index(0)
		, m_disabled(false)
	{ }

	ThreadsBarrier(const ThreadsBarrier&) = delete;
	ThreadsBarrier(ThreadsBarrier &&) = delete;
	ThreadsBarrier & operator=(const ThreadsBarrier&) = delete;
	ThreadsBarrier & operator=(ThreadsBarrier &&) = delete;

	// sync point
	void Await()
	{
		std::unique_lock<std::mutex> lock(m_requestsLock);
		if (m_disabled)
			return;

		unsigned currentIndex = m_index;
		++m_counts[currentIndex];

		// "spurious wakeup" means this thread could wake up even if no one called m_condition.notify!
		if (m_counts[currentIndex] < m_numThreads)
		{
			while (m_counts[currentIndex] < m_numThreads)
				m_condition.wait(lock);
		}
		else
		{
			m_index ^= 1; // flip index
			m_counts[m_index] = 0;
			m_condition.notify_all();
		}
	}

	// Call this to free current sleeping threads and prevent any further awaits.
	// After calling this, the object is no longer usable.
	void Break()
	{
		std::unique_lock<std::mutex> lock(m_requestsLock);
		m_disabled = true;
		m_counts[0] = m_numThreads;
		m_counts[1] = m_numThreads;
		m_condition.notify_all();
	}

private:
	std::mutex     m_requestsLock;
	std::condition_variable m_condition;
	const unsigned m_numThreads;
	unsigned       m_counts[2];
	unsigned       m_index;
	bool           m_disabled;
};