#pragma once

#include <functional>

namespace TinyThreadPool 
{
	using Job = std::function<void()>;

	void	Initialize();
	void	Shutdown();
	void	Execute(const Job& job);
	bool	Busy();
	void	Wait();
}