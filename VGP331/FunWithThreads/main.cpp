#include <iostream>
#include <thread>
#include <mutex>

const int kMax{ 10 };
std::mutex mtx;

void ThreadMain(int i)
{
	std::lock_guard<std::mutex> lock{ mtx };
	std::cout << "I am thread " << i << "\n";
}

int main(int argc, char* argv[])
{
	std::thread threads[kMax];
	for (uint32_t i{ 0 }; i < kMax; ++i)
	{
		threads[i] = std::thread{ ThreadMain, i };
	}

	{
		std::lock_guard<std::mutex> lock{ mtx };
		std::cout << "I am main thread\n";
	}

	for (uint32_t i{ 0 }; i < kMax; ++i)
	{
		threads[i].join();
	}

	return 0;
}