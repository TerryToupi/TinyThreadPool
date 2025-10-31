#include <TinyThreadPool.h>

#include <thread>
#include <string>
#include <sstream>
#include <iostream>

std::string get_thread_id()
{
    auto id = std::this_thread::get_id();
    std::stringstream ss;
    ss << id;
    return ss.str();
}

int main(void)
{
    TinyThreadPool::Initialize();

    std::cout << "Initializing ..." << std::endl;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
			TinyThreadPool::Execute([i, j]() 
			{
				printf("Batch ID: %d, Task ID: %d, With Worker ID: %s\n", i, j, get_thread_id().c_str());
			});

    TinyThreadPool::Wait();

    printf("\n");
    printf("\n");
    printf("\n");

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
			TinyThreadPool::Execute([i, j]() 
			{
				printf("Batch ID: %d, Task ID: %d, With Worker ID: %s\n", i, j, get_thread_id().c_str());
			});

    TinyThreadPool::Shutdown();
    return 0;
}