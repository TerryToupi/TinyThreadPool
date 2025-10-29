#include <TinyThreadPool.h>

#include <deque>
#include <atomic>
#include <thread>
#include <algorithm> 
#include <condition_variable>


namespace TinyThreadPool
{
	class JobQueue 
	{
	public:
		inline bool push_back(const Job& item)
		{
			std::scoped_lock lock(m_lock);
			m_queue.push_back(item);
			return true;
		}

		inline bool pop_front(Job& item)
		{
			std::scoped_lock lock(m_lock);
			if (m_queue.empty())
				return false;

			item = std::move(m_queue.front());
			m_queue.pop_front();
			return true;
		}

	private:
		std::deque<Job> m_queue;
		std::mutex		m_lock;
	};



	// Thread pool state
	uint32_t				p_num_threads = 0;
	uint64_t				p_current_label = 0;
	std::atomic<uint64_t>	p_finish_label;
	JobQueue				p_job_pool;
	std::condition_variable	p_wake_condition;
	std::mutex				p_wake_mutex;



	void init()
	{
		p_finish_label.store(0);

		auto cores = std::thread::hardware_concurrency();
		cores = std::max(1u, cores);

		for (uint32_t thread_id = 0; thread_id < cores; ++thread_id)
		{
			std::thread worker([] {
				Job job;
				while (true)
				{
					if (p_job_pool.pop_front(job))
					{
						job();
						p_finish_label.fetch_add(1);
					}
					else
					{
						std::unique_lock<std::mutex> lock(p_wake_mutex);
						p_wake_condition.wait(lock);
					}
				}
				});

			worker.detach();
		}
	}

	inline void poll()
	{
		p_wake_condition.notify_one();
		std::this_thread::yield();
	}

	void exec(const Job& job)
	{
		p_current_label += 1;

		while (!p_job_pool.push_back(job)) { poll(); }
		p_wake_condition.notify_one();
	}

	bool is_busy()
	{
		return p_finish_label.load() < p_current_label;
	}

	void wait()
	{
		while (is_busy()) { poll(); }
	}
}
