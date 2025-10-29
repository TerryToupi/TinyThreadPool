#pragma once

#include <functional>

namespace TinyThreadPool 
{
	using Job = std::function<void()>;

	void	init();
	void	exec(const Job& job);
	bool	is_busy();
	void	wait();
}