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
			std::scoped_lock locker(m_lock);
			m_queue.push_back(item);
			return true;
		}

		inline bool pop_front(Job& item)
		{
			std::scoped_lock locker(m_lock);
			if (m_queue.empty())
				return false;

			item = std::move(m_queue.front());
			m_queue.pop_front();
			return true;
		}

		inline bool clear()
		{
			std::scoped_lock locker(m_lock);
			m_queue.clear();
			return true;
		}

	private:
		std::deque<Job> m_queue;
		std::mutex		m_lock;
	};



	// Thread pool state
	std::atomic<bool>		 p_alive;
	std::vector<std::thread> p_threads;
	uint32_t				 p_num_threads = 0;
	std::atomic<uint64_t>	 p_current_label;
	std::atomic<uint64_t>	 p_finish_label;
	JobQueue				 p_job_pool;
	std::condition_variable	 p_wake_condition;
	std::mutex				 p_wake_mutex;



	void Initialize()
	{
		p_finish_label.store(0);
		p_current_label.store(0);

		auto cores = std::thread::hardware_concurrency();
		cores = std::max(1u, cores);

		p_threads.reserve(cores);

		p_alive.store(true);
		for (uint32_t thread_id = 0; thread_id < cores; ++thread_id)
		{
			p_threads.emplace_back([=] 
			{
				Job job;
				while (p_alive.load())
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
		}
	}

	void Shutdown()
	{
		Wait();

		p_alive.store(false);
		p_wake_condition.notify_all();

		for (auto& thread : p_threads)
		{
			if (thread.joinable())
				thread.join();
		}

		p_threads.clear();
		p_job_pool.clear();
		p_num_threads = 0;
		p_current_label.store(0);
		p_finish_label.store(0);
	}

	inline void poll()
	{
		p_wake_condition.notify_one();
		std::this_thread::yield();
	}

	void Execute(const Job& job)
	{
		p_current_label.fetch_add(1);
		while (!p_job_pool.push_back(job)) { poll(); }
		p_wake_condition.notify_one();
	}

	bool Busy()
	{
		return p_finish_label.load() < p_current_label.load();
	}

	void Wait()
	{
		while (Busy()) { poll(); }
	}
}
