#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <ctime>

std::mutex mtx;

void PrintProgressBar(const int& thread_id, int current, const int& length, bool& isComplete, double calcTime, const int& pb_length)
{
	int filled = static_cast<float>((float)current / length) * pb_length;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { filled + 17, thread_id + 1 };
	SetConsoleCursorPosition(hConsole, coord);
	if (isComplete)
	{
		std::cout << "\t" << calcTime;
	}
	else if (rand() % 10 == 1)
	{
		std::cout << "\033[31m" << static_cast<char>(219) << "\033[0m";
	}else
	{
		std::cout << static_cast<char>(219);
	}
}

void ProgressBar(const int& thread_id, const int& length)
{
	bool isComplete = 0;
	int pb_length = 25;
	srand(thread_id + time(NULL));
	auto timeStart = std::chrono::steady_clock::now();
	std::unique_lock<std::mutex> ul(mtx);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, thread_id + 1 };
	SetConsoleCursorPosition(hConsole, coord);
	std::cout << thread_id << "\t" << std::this_thread::get_id();
	ul.unlock();
	for (int i = 0; i < length; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
		ul.lock();
		if (static_cast<int>((float)i / length * pb_length) < static_cast<int>((float)(i + 1) / length * pb_length))
			PrintProgressBar(thread_id, i, length, isComplete, .0, pb_length);
		ul.unlock();
	}
	isComplete = 1;
	auto timeEnd = std::chrono::steady_clock::now();
	std::chrono::duration<double> calcTime = timeEnd - timeStart;
	ul.lock();
	PrintProgressBar(thread_id, length, length, isComplete, calcTime.count(), pb_length);
	ul.unlock();
}

int main()
{
	int count_of_threads = 5;
	int length = 50;

	std::cout << "#\tID\t\tProgress Bar\t\tTime" << std::endl;

	std::vector<std::thread> th(count_of_threads);
	for (int i = 0; i < count_of_threads; i++)
		th[i] = std::thread(ProgressBar, i, length);

	for (int i = 0; i < count_of_threads; i++)
		th[i].join();
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, count_of_threads + 1 };
	SetConsoleCursorPosition(hConsole, coord);

	return 0;
}
